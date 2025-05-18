#include "esp_log.h"
#include <my_system.h>
#include <input_output.h>
#include <my_fingerprint.h>
#include <my_wifi.h>
#include <my_firebase.h>
// #include <my_rfid.h>
#include <my_rfid_v1.h>
#include <my_oled.h>
#include <network_time.h>

// TAG sử dụng khi debug hệ thống 
static const char *TAG = "SYSTEM";

uint8_t registerID = 0;       // chứa id sẽ đăng ký
uint8_t fingerID   = 0;       // chứa fingerID đọc được
uint8_t count_times = 0;      // đếm thời gian được 1 phút thì reload màn hình thời gian


void setup() {
  // khởi tạo hệ thống
  system_init();

  solenoidEnable(HIGH);
}


void loop() {
  if(millis() - last_time > 500) {
    // xử lý tiến trình
    system_process();
    last_time = millis();

    bool sta_solenoid = solenoidRead();
    ESP_LOGD(TAG, "INPUT SOLENOID %d", sta_solenoid );
    count_times++;

    if(count_times >= 180) {
      // vì 1 giây nó đọc 2 lần

      // hiển thị thời gian
      // oledClockCircle();
      // reset count time
      count_times = 0;
    }
  }
}

void system_init(void) {
  Serial.begin(57600);
  // oledInit();

  // wifi init
  // oledWiFiStart();
  wifi_init();
  // oledWiFiConnected();
  // hết wifi init

  // network time init
  networkTimeInit();

  // Init firebase
  firebaseInit(); 

  // peripheral & sensor init
  gpioInit();
  fingerInit();
  rfid_init();  

  // print quantity fingerprint
  fingerQtyTemplate();
  fingerClearAll();
  rfid_clear_all();
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

    // Lấy thời gian mở cửa
    int timestamp = getTime();

    // Mở cửa
    solenoidEnable(HIGH);

    // Đọc tín hiệu phản hồi

    /*  Tạo path, ví dụ fingerID = 7
        path: /isOpen/fingerID-7
    */
    String pathSub = "/isOpen/fingerID-";
    pathSub.concat(fingerID);


    // Gửi lên firebase thông báo mở cửa - true sau này sẽ thay bằng tín hiệu mở cửa thành công hay thất bại
    // fbSendBoolean("Store/", pathSub, true);
    fbSendInt("Store/", pathSub, timestamp);
  }

  // Đọc xem có thẻ RFID nào không ?
  if(rfid_avaiable() == RFID_SUCCESS) {
    rfid_save_buffer();
  }
}

void system_register(void) {
  // Đăng ký vân tay
  uint8_t finger_status = fingerEnroll(registerID);

  // chờ 500ms
  delay(500);  

  // Nếu đăng ký vân tay thành công
  if(finger_status == FINGERPRINT_OK) {
    // gửi tín hiệu đăng ký thành công lên firebase (dạng chuỗi)
    // PATH: /Store/$uid/template/fingerID
    fbSendtring("Store/", "/template/fingerID", String(registerID));

    // LED thông báo finger đăng ký hoàn tất
    ledGreenEnable(HIGH);
    delay(2000);
    ledGreenEnable(LOW);
  }
  
  
  // Đăng ký RFID
  String regisRFID = "";

  // Chờ quét thẻ rfid
  while(rfid_avaiable() != RFID_SUCCESS) {
    delay(50);
  }

  rfid_save_buffer();

  uint8_t rfid_status = rfid_enroll(&regisRFID);
  if(rfid_status == RFID_SUCCESS) {
    fbSendtring("Store/", "/template/rfID", regisRFID);

    // LED thông báo rfid đăng ký hoàn tất
    ledGreenEnable(HIGH);
    delay(2000);
    ledGreenEnable(LOW);
  }

  // nếu bộ nhớ lưu trữ uid của rfid đầy 
  else if(rfid_status == RFID_EEPROM_FULL) {
    // ...
  }


  // Chờ thủ tục đăng ký hoàn tất
  // Hiển thị processbar - hoàn thành 95%
  oledProcessBar(95, 10, 20, 10);
  oledText("Wating ...", 50, 50);

  bool blink_led = false;
  while(fbGetSignalAddUser(&registerID)) {
    // chớp tắt led xanh
    ledGreenEnable(blink_led);
    blink_led = !blink_led;

    delay(300);
  }

  // Hiển thị đã hoàn tất đăng ký
  ledGreenEnable(HIGH);   
  buzzerEnable(HIGH);

  oledClearAll();
  oledProcessBar(100, 10, 20, 10);
  oledText("Success", 50, 50);

  delay(1000);

  ledGreenEnable(LOW);
  buzzerEnable(LOW);

  // Hoàn thành quá trình đăng ký
  // Quay về trạng thái IDLE chờ quét vân tay / rfid để mở cửa
  system_state = IDLE;
}