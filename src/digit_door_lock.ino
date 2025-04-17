#include "esp_log.h"
#include <my_system.h>
#include <input_output.h>
#include <my_fingerprint.h>
#include <my_wifi.h>
#include <my_firebase.h>
#include <my_rfid.h>
#include <my_oled.h>
#include <network_time.h>

// TAG sử dụng khi debug hệ thống 
static const char *TAG = "SYSTEM";


uint8_t registerID = 0;       // chứa id sẽ đăng ký
uint8_t fingerID   = 0;       // chứa fingerID đọc được
String  fingerTemplate = "";  // the real template

void setup() {
  // khởi tạo hệ thống
  system_init();
}

void loop() {
  if(millis() - last_time > ONE_SECOND) {
    system_process();
    last_time = millis();
  }
}

void system_init(void) {
  Serial.begin(57600);
  oledInit();

  // wifi init
  oledWiFiStart();
  wifi_init();
  oledWiFiConnected();
  // hết wifi init

  // network time init
  networkTimeInit();

  // Init firebase
  firebaseInit(); 

  // peripheral & sensor init
  gpioInit();
  fingerInit();

  // print quantity fingerprint
  fingerQtyTemplate();
}

void system_process(void) {
  // Debug Log hiển thị trạng thái hiện tại của hệ thống
  ESP_LOGD(TAG, "%s",system_name[system_state].c_str());

  // Timestamp của NTP
  // int timestamp = getTime();
  // ESP_LOGD(TAG, "Timestamp %d", timestamp);

  // process
  switch(system_state) {
    // Trạng thái bình thường (chờ đọc vân tay / rfid / tín hiệu điều khiển)
    case IDLE:
      system_idle();
      break;
    
    // Trạng thái THÊM NGƯỜI DÙNG MỚI
    case REGISTER:
      system_register();
      break;

    default:
      break;
  } 
}

void system_idle(void) {
  // Nếu có tín hiệu THÊM NGƯỜI DÙNG MỚI thì sẽ chuyển sang trạng thái đăng ký
  // PATH: /Store/$uid/register/nextFingerID
  if(fbGetSignalAddUser(&registerID)) {
    system_state = REGISTER;
  }


  // Đọc xem có vân tay nào quét không
  if(getFingerprintID(&fingerID) == FINGERPRINT_OK) {
    // Nếu đọc vân tay thành công

    // Mở cửa
    solenoidEnable(HIGH);

    // Đọc tín hiệu phản hồi

    /*  Tạo path, ví dụ fingerID = 7
        path: /isOpen/finger7
    */
    String pathSub = "/isOpen/fingerID-";
    pathSub.concat(fingerID);

    // Gửi lên firebase thông báo mở cửa - true sau này sẽ thay bằng tín hiệu mở cửa thành công hay thất bại
    fbSendBoolean("Store/", pathSub, true);
  }
}

void system_register(void) {
  // Đăng ký vân tay
  uint8_t finger_status = fingerEnroll(registerID);

  // chờ 50ms
  delay(500);  

  // Nếu đăng ký vân tay thành công
  if(finger_status == FINGERPRINT_OK) {
    // gửi tín hiệu đăng ký thành công lên firebase (dạng chuỗi)
    // PATH: /Store/$uid/template/fingerID
    fbSendtring("Store/", "/template/fingerID", String(registerID));
  }
  
  // Chờ thủ tục đăng ký hoàn tất
  while(fbGetSignalAddUser(&registerID)) {
    // Hiển thị processbar - hoàn thành 95%
    oledProcessBar(95, 10, 20, 10);
    oledText("Wating ...", 50, 50);
    delay(100);
  }

  // Hiển thị đã hoàn tất đăng ký
  oledClearAll();
  oledProcessBar(100, 10, 20, 10);
  oledText("Success", 50, 50);

  // Hoàn thành quá trình đăng ký
  // Quay về trạng thái IDLE chờ quét vân tay / rfid để mở cửa
  system_state = IDLE;
}