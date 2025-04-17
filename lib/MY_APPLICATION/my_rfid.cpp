#include <my_rfid.h>

void rfid_init(void) {
    mfrc522.PCD_Init();    // Init MFRC522 board.
    MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
    Serial.println("RFID đã kết nối thành công"); // CẦN FIX LẠI CHỔ NÀY
}

