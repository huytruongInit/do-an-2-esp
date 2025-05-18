#include <my_rfid_v1.h>

static const char *TAG = "RFID";

// Khởi tạo RFID
void rfid_init(void) {
    EEPROM.begin(EEPROM_SIZE); // Khởi tạo EEPROM

    SPI.begin();
    rfid.PCD_Init();

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

// Lưu UID vào buffer
void rfid_save_buffer(void) {
    for (byte i = 0; i < RFID_CARD_SIZE; i++) {
        readNUID[i] = rfid.uid.uidByte[i];
    }
}

// Xóa toàn bộ data rfid đã lưu trong eeprom
void rfid_clear_all(void) {
    for (int i = RFID_START_IN_MEMORY; i < RFID_MAX_CARDS; i++) {
        for (int j = 0; j < RFID_CARD_SIZE; j++) {
            int address_eeprom = i * RFID_CARD_SIZE + j;
            EEPROM.write(address_eeprom, 0xFF);
        }
    }

    ESP_LOGD(TAG, "EEPROM Clear All");
}

// Kiểm tra có thẻ mới không
uint8_t rfid_avaiable(void) {
    ESP_LOGD(TAG, "No rfid detect");
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
        return RFID_FAIL;
    }

    return RFID_SUCCESS;
}

// Đăng ký RFID ~ Lưu vào EEPROM
uint8_t rfid_enroll(String *uRFID) {
    for (int i = RFID_START_IN_MEMORY; i < RFID_MAX_CARDS; i++) {
        // Kiểm tra xem bộ nhớ tại vị trí i có đang trống không
        bool empty = true;
        for (int j = 0; j < RFID_CARD_SIZE; j++) {
            if (EEPROM.read(i * RFID_CARD_SIZE + j) != 0xFF) {
                empty = false;
                break;
            }
        }

        // Nếu bộ nhớ trống
        if (empty) {
            for (int j = 0; j < RFID_CARD_SIZE; j++) {

                // Lưu trong eeprom thì sẽ lưu theo chiều thuận của UID
                EEPROM.write(i * RFID_CARD_SIZE + j, readNUID[j]);

                // Lưu ngược
                *uRFID += *(readNUID + (RFID_CARD_SIZE - j - 1));
            }
            EEPROM.commit(); // Nếu dùng ESP32
            return RFID_SUCCESS;
        }
    }
    ESP_LOGD(TAG, "RFID: Bộ nhớ RFID đã đầy");
    return RFID_EEPROM_FULL;
}
