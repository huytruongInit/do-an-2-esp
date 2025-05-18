#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

// Định nghĩa chân SPI - RFID
#define SS_PIN 5
#define RST_PIN 4

// Định số lượng card và kích thước của 1 card
#define EEPROM_SIZE     512         // Kích thước bộ nhớ EEPROM
#define RFID_MAX_CARDS  50          // Số lượng thẻ tối đa bạn muốn lưu
#define RFID_CARD_SIZE  4           // Kích thước UID của thẻ

#define RFID_START_IN_MEMORY  0     // Nơi bắt đầu lưu trữ trong bộ nhớ
// Các trạng thái RFID
#define RFID_SUCCESS     0
#define RFID_FAIL        1
#define RFID_EEPROM_FULL 2

static MFRC522 rfid(SS_PIN, RST_PIN);
static MFRC522::MIFARE_Key key;
static byte readNUID[RFID_CARD_SIZE];

void rfid_init(void);                // Khởi tạo RFID
void rfid_save_buffer(void);         // Lưu UID thẻ vào biến tạm
void rfid_clear_all(void);           // Xóa toàn bộ data rfid đã lưu trữ trong eeprom

uint8_t rfid_avaiable(void);         // Kiểm tra có thẻ mới không
uint8_t rfid_enroll(String *uRFID);  // Đăng ký RFID ~ Lưu vào eeprom esp32