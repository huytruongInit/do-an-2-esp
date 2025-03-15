#ifndef MY_RFID
#define MY_RFID

// Thư viện kết nối rfid
/**
 *  SDA:    5
 *  SCK:    18
 *  MOSI:   23
 *  MISO:   19
 *  IRQ:    NC 
 *  GND:    GND
 *  RST:    21
 *  VCC:    3.3V
*/

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

void rfid_init(void) {
    mfrc522.PCD_Init();    // Init MFRC522 board.
    MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
    Serial.println("RFID đã kết nối thành công"); // CẦN FIX LẠI CHỔ NÀY
}

#endif