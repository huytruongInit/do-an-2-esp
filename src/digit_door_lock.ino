#include <secret.h>
#include <input_output.h>
#include <my_wifi.h>
#include <my_firebase.h>
#include <my_fingerprint.h>
#include <my_rfid.h>

void setup() {
  Serial.begin(57600);
  io_init();
  wifi_smart_config();
  firebase_init();
  finger_init();
  rfid_init();
  // oled_init();
}

void loop() {

}

/**
 * Note: 
  Nếu tín hiệu cảm biến giữa chừng không nhận nữa thì phải làm sao để detect là cảm biến không còn kết nối ? Watch dog timer
  Chưa test thử nếu có 2 app, hoặc 2 con esp nằm gần nhau thì sẽ kết nối như thế nào (smart config)
*/

