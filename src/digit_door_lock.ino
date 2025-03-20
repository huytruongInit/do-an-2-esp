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
uint16_t  count_time = 0;   // ...

// ĐỊNH NGHĨA CÁC TRẠNG THÁI CỦA HỆ THỐNG
typedef enum {
  NORMAL    = 0,    // bình thường, luôn chờ đọc vân tay
  REGISTER  = 1,    // thêm vân tay (thêm người vào nhà)
} SystemState;
SystemState system_state = NORMAL;

// KHAI BÁO CÁC HÀM CỦA HỆ THỐNG
void system_run(void);        
uint8_t readnumber(void);

void setup() {
  Serial.begin(57600);

  // network init
  wifi_init();
  firebase_init();

  // peripheral & sensor init
  io_init();
  fingerInit();
  fingerQtyTemplate();  // in ra số lượng template vân tay mà cảm biến đang lưu trữ
  rfid_init();
  // oled_init();

  
}

void loop() {
  // Non-blocking
  if(millis() - last_time >= 2000) {
    system_run();

    count_time++;
    if(count_time > 5) {
      system_state = REGISTER;
    }

    last_time = millis();
  }
}

uint8_t readnumber(void) {
  uint8_t num = 0;

  while (num == 0) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

// ĐỊNH NGHĨA HÀM CỦA HỆ THỐNG
void system_run(void) {
  switch(system_state) {
    case NORMAL:
      Serial.println("Trạng thái hệ thống: Bình thường");
      // ... chờ đọc vân tay || ... chờ đọc rfid
      break;


    case REGISTER:
      Serial.println("Trạng thái hệ thống: Thêm vân tay");
      uint8_t id =readnumber();       // Tạo ID (Giả lập, ID sau này sẽ gửi từ web xuống luôn)

      uint8_t p = fingerEnroll(id);   // đăng ký vân tay mới
      if(p != FINGERPRINT_OK) Serial.println("Đăng ký vân tay thất bại");
      fingerDownloadTemplate(id);     // Lấy template vân tay
      
      // Lấy template gửi lên firebase

      // rfidEnroll     // đăng ký rfid luôn
      // Lấy Template gửi lên firebase

      // Nếu enroll thành công thì lấy template của finger gửi lên web
      // ...
      break;


    // default: 
    //   break;  
    // ...

  }

}