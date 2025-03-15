#include <secret.h>
#include <input_output.h>
#include <my_wifi.h>
#include <my_firebase.h>
#include <my_fingerprint.h>
#include <my_rfid.h>

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

#define SYSTEM_STATE_NORMAL   0
#define SYSTEM_STATE_REGISTER 1
uint8_t system_state = SYSTEM_STATE_NORMAL;

void system_run(uint8_t state) {
  switch(state) {
    case SYSTEM_STATE_NORMAL:
      // chạy bth...
      Serial.println("Trạng thái bình thường");
      break;

    case SYSTEM_STATE_REGISTER:
      // đăng ký
      Serial.println("Vân tay - Đăng ký!");
        Serial.println("Vân tay - Vui lòng nhập ID # (từ 1 đến 127) bạn muốn lưu trữ ...");
        id = readnumber();
        if (id == 0) {// ID #0 not allowed, try again!
           return;
        }
        Serial.print("Vân tay -  Đăng ký ID #");
        Serial.println(id);

        while (! getFingerprintEnroll() );
      break;

    default:
      break;
  }
}

long long last_time = 0;
#define ONE_SECOND 1000

void loop() {
  system_run(system_state);
  if(millis() - last_time >= ONE_SECOND * 45) {
    system_state++;
    system_state %= 2;
    last_time = millis();
  }
}

/**
 * Note: 
  Nếu tín hiệu cảm biến giữa chừng không nhận nữa thì phải làm sao để detect là cảm biến không còn kết nối ? Watch dog timer
  Chưa test thử nếu có 2 app, hoặc 2 con esp nằm gần nhau thì sẽ kết nối như thế nào (smart config)
*/

