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

HardwareSerial mySerial(0); // sử dụng uart0 (hardware)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void finger_init(void) {
    finger.begin(57600);
  
    if (finger.verifyPassword()) {
      Serial.println("Vân tay kết nối thành công");
    } else {
      Serial.println("Vân tay kết nối thất bại :(");
      while (1) { delay(1); } // CẦN FIX LẠI CHỔ NÀY
    }
}

#endif