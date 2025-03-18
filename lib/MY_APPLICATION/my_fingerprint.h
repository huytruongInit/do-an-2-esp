#ifndef MY_FINGERPRINT
#define MY_FINGERPRINT

// Thư viện kết nối vân tay
/**
 *  VCC: 3.3V 
 *  GND: GND
 *  TX:  RX
 *  RX:  TX 
*/
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

static HardwareSerial mySerial(0); // sử dụng uart0 (hardware)
static Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void finger_init(void);
uint8_t finger_readnumber(void);
uint8_t getFingerprintEnroll(uint8_t id);

#endif