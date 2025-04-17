#include <my_fingerprint.h>
static const char *TAG = "FINGER";

// -----------------------------------------------------------------------------------------------------------------------------------------------  //
// ------------------------------------------------------------- PUBLIC -------------------------------------------------------------------------  //
void fingerInit(void) {
    SerialFinger.begin(57600, SERIAL_8N1, 12, 13);   // 12 RX, 13 TX
    finger.begin(57600);

    // kiểm tra xem có kết nối được đến module vân tay không
    if (finger.verifyPassword()) {
      ESP_LOGD(TAG, "Connection success");
    } 
    else {
      ESP_LOGD(TAG, "Connection failed");
      while (1) { delay(1); } // CẦN FIX LẠI CHỔ NÀY
    }

    // finger.emptyDatabase();     // Xóa toàn bộ vân tay
}

void fingerQtyTemplate(void) {
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    ESP_LOGD(TAG, "Sensor doesn't contain any fingerprint data.");
  }
  else {
    ESP_LOGD(TAG, "Waiting for valid finger...");
    ESP_LOGD(TAG, "Sensor contains %d templates", finger.templateCount);
  }
}

uint8_t fingerEnroll(uint8_t id) {
    int p = -1;

    // Hiển thị oled quét vân tay lần 1
    oledFingerScan(60, 5);
    oledText("1st Scan Finger", 0, 0);

    ESP_LOGD(TAG, "Waiting for valid finger to enroll as # %d", id);
    fingerGetImage(1);                      // đọc ảnh vân tay lần 1
    p = fingerConvert(1);                   // convert ảnh vân tay lần 1
    if(p != FINGERPRINT_OK)     return p;   // convert ảnh thất bại

    // Đọc vân tay lần 1 thành công, thả vân tay ra 
    oledFingerValid(70, 0);
    oledText("Scan success", 0, 0);
    oledText("Remove finger", 0, 15);

    // Thả tay ra 2 giây
    ESP_LOGD(TAG, "Remove finger");
    delay(2000);

    // Hiển thị oled quét vân tay lần 2
    oledFingerScan(60, 5);
    oledText("2nd Scan Finger", 0, 0);

    fingerGetImage(2);                      // đọc ảnh vân tay lần 2
    p = fingerConvert(2);                   // convert ảnh vân tay lần 2
    if(p != FINGERPRINT_OK)     return p;   // convert ảnh thất bại

    // Đọc vân tay lần 2 thành công, thả vân tay ra 
    oledFingerValid(70, 0);
    oledText("Scan success", 0, 0);
    oledText("Remove finger", 0, 15);
    oledText("Wait a second", 0, 30);

    // Tạo model
    // Serial.print("Creating model for #");  Serial.println(id);
    p = fingerCreateModel();                // tạo model template cho vân tay
    if(p != FINGERPRINT_OK)     return p;   // tạo model template thất bại

    // Lưu model vào store của as608
    // Serial.print("ID "); Serial.println(id);
    p = fingerStoreModel(id);
    if(p != FINGERPRINT_OK)     return p;   // lưu trữ model template thất bại

    return FINGERPRINT_OK;                  // đăng ký vân tay thành công
}

uint8_t fingerDownloadTemplate(uint8_t id, String *fingerTemplateFinal) {
    ESP_LOGD(TAG, "Attempting to load #%d", id);

    int p = fingerLoadModel(id);        // load vân tay từ model của as608
    if(p != FINGERPRINT_OK) return p;   // load thất bại

    ESP_LOGD(TAG, "Attempting to get #%d", id);
    p = fingerGetModel(id);             // get model
    if(p != FINGERPRINT_OK) return p;   // get thất bại

    // one data packet is 267 bytes. in one data packet, 11 bytes are 'usesless' :D
    // 
    uint8_t bytesReceived[534];         // 2 data packets
    memset(bytesReceived, 0xff, 534); 

    // đọc dữ liệu
    uint32_t starttime = millis();
    int i = 0;
    while (i < 534 && (millis() - starttime) < 20000) {
      if (SerialFinger.available()) {
        bytesReceived[i++] = SerialFinger.read();
      }
    }

    ESP_LOGD(TAG, "%d bytes read.", i);
    ESP_LOGD(TAG, "Decoding packet...");

    uint8_t fingerTemplate[512]; // the real template
    memset(fingerTemplate, 0xff, 512);
  
    // filtering only the data packets
    int uindx = 9, index = 0;
    memcpy(fingerTemplate + index, bytesReceived + uindx, 256);   // first 256 bytes
    uindx += 256;       // skip data
    uindx += 2;         // skip checksum
    uindx += 9;         // skip next header
    index += 256;       // advance pointer
    memcpy(fingerTemplate + index, bytesReceived + uindx, 256);   // second 256 bytes
    

    for (int i = 0; i < 512; ++i) {
      *fingerTemplateFinal += String(fingerTemplate[i]);
    }
    ESP_LOGD(TAG, "DT: %s", (*fingerTemplateFinal).c_str());

    return p;
}

uint8_t fingerDeleteTemplate(uint8_t id) {
    uint8_t p = -1;

    p = finger.deleteModel(id);
  
    if (p == FINGERPRINT_OK) {
      ESP_LOGD(TAG, "Deleted!");
    } 
    else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      ESP_LOGD(TAG, "Communication error");
    } 
    else if (p == FINGERPRINT_BADLOCATION) {
      ESP_LOGD(TAG, "Could not delete in that location");
    } 
    else if (p == FINGERPRINT_FLASHERR) {
      ESP_LOGD(TAG, "Error writing to flash");
    } 
    else {
      // Serial.println(p, HEX);
      ESP_LOGD(TAG, "Unknown error: 0x");
    }
    
    return p;
}

uint8_t getFingerprintID(uint8_t *fingerID) {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      ESP_LOGD(TAG, "Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      ESP_LOGD(TAG, "No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ESP_LOGD(TAG, "Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      ESP_LOGD(TAG, "Imaging error");
      return p;
    default:
      ESP_LOGD(TAG, "Unknown error");
      return p;
  }

  // OK success!
  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      ESP_LOGD(TAG, "Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      ESP_LOGD(TAG, "Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      ESP_LOGD(TAG, "Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      ESP_LOGD(TAG, "Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      ESP_LOGD(TAG, "Could not find fingerprint features");
      return p;
    default:
      ESP_LOGD(TAG, "Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    ESP_LOGD(TAG, "Found a print match!");
  } 
  else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    ESP_LOGD(TAG, "Communication error");
    return p;
  } 
  else if (p == FINGERPRINT_NOTFOUND) {
    ESP_LOGD(TAG, "Did not find a match");
    return p;
  } 
  else {
    ESP_LOGD(TAG, "Unknown error");
    return p;
  }

  // found a match!
  ESP_LOGD(TAG, "Found ID #%d with confidence of %d", finger.fingerID, finger.confidence);
  *fingerID = finger.fingerID;

  return FINGERPRINT_OK;
}

void fingerClearAll(void) {
  finger.emptyDatabase();
}
// -----------------------------------------------------------------------------------------------------------------------------------------------  //
// ------------------------------------------------------------- PRIVATE -------------------------------------------------------------------------  //
static void fingerGetImage(uint8_t slot) {
  int p = -1;

  // đọc ảnh vân tay lần 2
  if(slot == 2) {
      p = 0;
      while (p != FINGERPRINT_NOFINGER) p = finger.getImage();

      // Serial.print("ID "); Serial.println(id);
      p = -1;
      ESP_LOGD(TAG, "Place same finger again");
  }

  while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
          case FINGERPRINT_OK:
              ESP_LOGD(TAG, "Remove finger""Image taken");
              break;
          case FINGERPRINT_NOFINGER:
              ESP_LOGD(TAG, ".");
              break;
          case FINGERPRINT_PACKETRECIEVEERR:
              ESP_LOGD(TAG, "Communication error");
              break;
          case FINGERPRINT_IMAGEFAIL:
              ESP_LOGD(TAG, "Imaging error");
              break;
          default:
              ESP_LOGD(TAG, "Unknown error");
              break;
      }
  }
}

static int fingerConvert(uint8_t slot) {
  int p = -1;
  p = finger.image2Tz(slot);
  switch (p) {
      case FINGERPRINT_OK:
          ESP_LOGD(TAG, "Image converted");
          break;
      case FINGERPRINT_IMAGEMESS:
          ESP_LOGD(TAG, "Image too messy");
          break;
      case FINGERPRINT_PACKETRECIEVEERR:
          ESP_LOGD(TAG, "Communication error");
          break;
      case FINGERPRINT_FEATUREFAIL:
          ESP_LOGD(TAG, "Could not find fingerprint features");
          break;
      case FINGERPRINT_INVALIDIMAGE:
          ESP_LOGD(TAG, "Could not find fingerprint features");
          break;
      default:
          ESP_LOGD(TAG, "Unknown error");
          break;
  }
  return p;
}

static int fingerCreateModel(void) {
  int p = finger.createModel();
  switch(p) {
      case FINGERPRINT_OK:
          ESP_LOGD(TAG, "Prints matched!");
          break;

      case FINGERPRINT_PACKETRECIEVEERR:
          ESP_LOGD(TAG, "Communication error");
          break;

      case FINGERPRINT_ENROLLMISMATCH:
          ESP_LOGD(TAG, "Fingerprints did not match");
          break;

      default:
          ESP_LOGD(TAG, "Unknown error");
          break;
  }
  return p;
}

static int fingerStoreModel(uint8_t id) {
  int p = finger.storeModel(id);
  switch(p) {
      case FINGERPRINT_OK:
          ESP_LOGD(TAG, "Stored!");
          break;

      case FINGERPRINT_PACKETRECIEVEERR:
          ESP_LOGD(TAG, "Communication error");
          break;

      case FINGERPRINT_BADLOCATION:
          ESP_LOGD(TAG, "Could not store in that location");
          break;

      case FINGERPRINT_FLASHERR:
          ESP_LOGD(TAG, "Error writing to flash");
          break;

      default:
          ESP_LOGD(TAG, "Unknown error");
          break;
  }
  return p;
}

static int fingerLoadModel(uint8_t id) {
    uint8_t p = finger.loadModel(id);
    switch (p) {
      case FINGERPRINT_OK:
        ESP_LOGD(TAG, "Template %d loaded", id);
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        ESP_LOGD(TAG, "Communication error");
        break;
      default:
        ESP_LOGD(TAG, "Unknown error ");
        break;
    }
    return p;
}

static int fingerGetModel(uint8_t id) {
    int p = finger.getModel();
    switch (p) {
        case FINGERPRINT_OK:
            ESP_LOGD(TAG, "Template %d transferring", id);
            break;
        default:
            ESP_LOGD(TAG, "Unknown error ");
            break;
    }
    return p;
}

static void printHex(int num, int precision) {
    char tmp[16];
    char format[128];
  
    sprintf(format, "%%.%dX", precision);
  
    sprintf(tmp, format, num);
    Serial.print(tmp);
    // ESP_LOGD(TAG, "Template HEX: %s", tmp);
}