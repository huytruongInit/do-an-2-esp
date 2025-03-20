#include <my_fingerprint.h>

// -----------------------------------------------------------------------------------------------------------------------------------------------  //
// ------------------------------------------------------------- PUBLIC -------------------------------------------------------------------------  //
void fingerInit(void) {
    SerialFinger.begin(57600, SERIAL_8N1, 12, 13);   // 12 RX, 13 TX
    finger.begin(57600);

    // kiểm tra xem có kết nối được đến module vân tay không
    if (finger.verifyPassword()) {
      Serial.println("Vân tay kết nối thành công");
    } else {
      Serial.println("Vân tay kết nối thất bại :(");
      while (1) { delay(1); } // CẦN FIX LẠI CHỔ NÀY
    }
}

void fingerQtyTemplate(void) {
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    // Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
    Serial.print("Cảm biến hiện tại không chứa bất kỳ dữ liệu vân tay nào");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
}

uint8_t fingerEnroll(uint8_t id) {
    int p = -1;

    Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
    fingerGetImage(1);                      // đọc ảnh vân tay lần 1
    p = fingerConvert(1);                   // convert ảnh vân tay lần 1
    if(p != FINGERPRINT_OK)     return p;   // convert ảnh thất bại

    // Thả tay ra 2 giây
    Serial.println("Remove finger");
    delay(2000);

    fingerGetImage(2);                      // đọc ảnh vân tay lần 2
    p = fingerConvert(2);                   // convert ảnh vân tay lần 2
    if(p != FINGERPRINT_OK)     return p;   // convert ảnh thất bại

    // Tạo model
    Serial.print("Creating model for #");  Serial.println(id);
    p = fingerCreateModel();                // tạo model template cho vân tay
    if(p != FINGERPRINT_OK)     return p;   // tạo model template thất bại

    // Lưu model vào store của as608
    Serial.print("ID "); Serial.println(id);
    p = fingerStoreModel(id);
    if(p != FINGERPRINT_OK)     return p;   // lưu trữ model template thất bại

    return FINGERPRINT_OK;                  // đăng ký vân tay thành công
}

uint8_t fingerDownloadTemplate(uint8_t id) {
    Serial.println("------------------------------------");
    Serial.print("Attempting to load #"); Serial.println(id);
    
    int p = fingerLoadModel(id);        // load vân tay từ model của as608
    if(p != FINGERPRINT_OK) return p;   // load thất bại

    Serial.print("Attempting to get #"); Serial.println(id);
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

    Serial.print(i); Serial.println(" bytes read.");
    Serial.println("Decoding packet...");

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
      //Serial.print("0x");
      printHex(fingerTemplate[i], 2);
      //Serial.print(", ");
    }
    Serial.println("\ndone.");
  
    return p;
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
      Serial.println("Place same finger again");
  }

  while (p != FINGERPRINT_OK) {
      p = finger.getImage();
      switch (p) {
          case FINGERPRINT_OK:
              Serial.println("Image taken");
              break;
          case FINGERPRINT_NOFINGER:
              Serial.print(".");
              break;
          case FINGERPRINT_PACKETRECIEVEERR:
              Serial.println("Communication error");
              break;
          case FINGERPRINT_IMAGEFAIL:
              Serial.println("Imaging error");
              break;
          default:
              Serial.println("Unknown error");
              break;
      }
  }
}

static int fingerConvert(uint8_t slot) {
  int p = -1;
  p = finger.image2Tz(slot);
  switch (p) {
      case FINGERPRINT_OK:
          Serial.println("Image converted");
          break;
      case FINGERPRINT_IMAGEMESS:
          Serial.println("Image too messy");
          break;
      case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
      case FINGERPRINT_FEATUREFAIL:
          Serial.println("Could not find fingerprint features");
          break;
      case FINGERPRINT_INVALIDIMAGE:
          Serial.println("Could not find fingerprint features");
          break;
      default:
          Serial.println("Unknown error");
          break;
  }

  return p;
}

static int fingerCreateModel(void) {
  int p = finger.createModel();
  switch(p) {
      case FINGERPRINT_OK:
          Serial.println("Prints matched!");
          break;

      case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;

      case FINGERPRINT_ENROLLMISMATCH:
          Serial.println("Fingerprints did not match");
          break;

      default:
          Serial.println("Unknown error");
          break;
  }
  return p;
}

static int fingerStoreModel(uint8_t id) {
  int p = finger.storeModel(id);
  switch(p) {
      case FINGERPRINT_OK:
          Serial.println("Stored!");
          break;

      case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;

      case FINGERPRINT_BADLOCATION:
          Serial.println("Could not store in that location");
          break;

      case FINGERPRINT_FLASHERR:
          Serial.println("Error writing to flash");
          break;

      default:
          Serial.println("Unknown error");
          break;
  }
  return p;
}

static int fingerLoadModel(uint8_t id) {
    uint8_t p = finger.loadModel(id);
    switch (p) {
        case FINGERPRINT_OK:
          Serial.print("Template "); Serial.print(id); Serial.println(" loaded");
          break;
        case FINGERPRINT_PACKETRECIEVEERR:
          Serial.println("Communication error");
          break;
        default:
          Serial.print("Unknown error "); Serial.println(p);
          break;
    }
    return p;
}

static int fingerGetModel(uint8_t id) {
    int p = finger.getModel();
    switch (p) {
        case FINGERPRINT_OK:
            Serial.print("Template "); Serial.print(id); Serial.println(" transferring:");
            break;
        default:
            Serial.print("Unknown error "); Serial.println(p);
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
}