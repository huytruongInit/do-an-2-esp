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
 *  RST:    17
 *  VCC:    3.3V
*/

#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

#define RST_PIN 17
#define SS_PIN  5

static MFRC522DriverPinSimple ss_pin(SS_PIN);
static MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
static MFRC522 mfrc522{driver};         // Create MFRC522 instance

void rfid_init(void);

#endif