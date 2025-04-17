#ifndef MY_FINGERPRINT
#define MY_FINGERPRINT

// Thư viện kết nối vân tay
/**
 *  AS608     ESP32
 * -----------------
 *  VCC:      3.3V 
 *  GND:      GND
 *  TX:       12
 *  RX:       13 
*/
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <my_oled.h>

// Uart0
// static HardwareSerial mySerial(0); // sử dụng uart0 (hardware)
// static Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Uart1
static HardwareSerial SerialFinger(1); // Khởi tạo Serial1
static Adafruit_Fingerprint finger = Adafruit_Fingerprint(&SerialFinger);

// -----------------------------------------------------------------------------------------------------------------------------------------------  //
// ------------------------------------------------------------- PUBLIC -------------------------------------------------------------------------  //
void fingerInit(void);                      // Khởi tạo cảm biến vân tay
void fingerQtyTemplate(void);               // In số lượng template vân tay mà cảm biến đang lưu trữ
uint8_t fingerEnroll(uint8_t id);           // Đăng ký một vân tay mới
uint8_t fingerDownloadTemplate(uint8_t id, String *fingerTemplateFinal); // Download (Tải || Lấy) template vân tay theo id
uint8_t fingerDeleteTemplate(uint8_t id);   // Xóa vân tay theo id
uint8_t getFingerprintID(uint8_t *fingerStatus);             // đọc vân tay (verify)
void fingerClearAll(void);                   // Xóa toàn bộ vân tay trong module
// -----------------------------------------------------------------------------------------------------------------------------------------------  //
// ------------------------------------------------------------- PRIVATE -------------------------------------------------------------------------  //
static void fingerGetImage(uint8_t slot);     // Đọc ảnh vân tay 
static int fingerConvert(uint8_t slot);       // Convert ảnh vân tay 
static int fingerCreateModel(void) ;          // Tạo model cho ảnh vân tay
static int fingerStoreModel(uint8_t id);      // Lưu vân tay
static int fingerLoadModel(uint8_t id);       // Load (Tải) model vân tay
static int fingerGetModel(uint8_t id);        // Get model vân tay
static void printHex(int num, int precision); // In template model vân tay dạng hex
#endif