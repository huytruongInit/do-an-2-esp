#include <secret.h>
#include <input_output.h>
#include <my_fingerprint.h>
#include <my_wifi.h>
#include <my_firebase.h>
#include <my_oled.h>
#include <my_rfid.h>

// ĐỊNH NGHĨA CÁC BIẾN THỜI GIAN NON-BLOCKING
#define ONE_SECOND 1000     // 1000ms ~ 1s
long long last_time = 0;    // ...

// ĐỊNH NGHĨA CÁC TRẠNG THÁI CỦA HỆ THỐNG
typedef enum {
  NORMAL    = 0,    // bình thường, luôn chờ đọc vân tay
  REGISTER  = 1,    // thêm vân tay (thêm người vào nhà)
} SystemState;
SystemState system_state = NORMAL;

// KHAI BÁO CÁC HÀM CỦA HỆ THỐNG
void system_run(void);        

void setup() {
  Serial.begin(57600);
  io_init();
  wifi_init();
  // wifi_smart_config();
  firebase_init();
  finger_init();
  rfid_init();
  // oled_init();
}

void loop() {
  // Non-blocking
  if(millis() - last_time >= 2000) {
    system_run();
    last_time = millis();
  }
}

// ĐỊNH NGHĨA HÀM CỦA HỆ THỐNG
void system_run(void) {
  switch(system_state) {
    case NORMAL:
      Serial.println("Trạng thái hệ thống: Bình thường");
      break;


    case REGISTER:
      Serial.println("Trạng thái hệ thống: Thêm vân tay");

      // Tạo ID (Giả lập, ID sau này sẽ gửi từ web xuống luôn)
      uint8_t id = finger_readnumber();
      


      system_state = NORMAL;  // chuyển về trạng thái bình thường
      break;


    default: 
      // ...
      break;
  }

}






/**
 * Note: 
  Nếu tín hiệu cảm biến giữa chừng không nhận nữa thì phải làm sao để detect là cảm biến không còn kết nối ? Watch dog timer
  Chưa test thử nếu có 2 app, hoặc 2 con esp nằm gần nhau thì sẽ kết nối như thế nào (smart config)
*/

/**
 *  Xóa bớt các file thừa trong thư viện ngoại vi
 *  Cấu trúc lại file cpp
*/