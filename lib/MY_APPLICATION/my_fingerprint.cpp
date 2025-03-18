#include <my_fingerprint.h>
#include <pair.h>

// Khởi tạo cảm biến vân tay
void finger_init(void) {
    finger.begin(57600);
  
    if (finger.verifyPassword()) {
      Serial.println("Vân tay kết nối thành công");
    } else {
      Serial.println("Vân tay kết nối thất bại :(");
      while (1) { delay(1); } // CẦN FIX LẠI CHỔ NÀY
    }
}


uint8_t finger_readnumber(void) {
    uint8_t num = 0;
  
    while (num == 0) {
      while (! Serial.available());
      num = Serial.parseInt();
    }
    return num;
}


uint8_t finger_enroll(uint8_t id) {
  uint8_t finger_status = -1;
  Serial.print("Đang chờ đợi vân tay #"); Serial.println(id);

  while(!finger_status) {
    finger_status = finger.getImage();    // đọc dữ liệu vân tay

  }
} 

String state[] = {
  "Image taken",          // 0x00 FINGERPRINT_OK  
  "Communication error",  // 0x01 FINGERPRINT_PACKETRECIEVEERR
  ".",                    // 0x02 FINGERPRINT_NOFINGER ~ kiểu đang chờ vân tay á
  "Imaging error",        // 0x03 FINGERPRINT_IMAGEFAIL
  "",
  "",
  "Image too messy",      // 0x06 FINGERPRINT_IMAGEMESS
  "Could not find fingerprint features",  // 0x07 FINGERPRINT_FEATUREFAIL

};

pair<uint8_t, String> state[] = {
  {0x00, "Image taken"},
  {0x01, "Communication error"},
  {0x02, "."},        
  {0x03, "Imaging error"},
  {0x06, "Image too messy"},
  {0x07, "Could not find fingerprint features"},
};

uint8_t finger_getFingerprintEnroll(uint8_t id) {

    int p = -1;
    
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
  
    // OK success!
  
    p = finger.image2Tz(1);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");    ///
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
  
    Serial.println("Remove finger");
    delay(2000);
    p = 0;
    while (p != FINGERPRINT_NOFINGER) {
      p = finger.getImage();
    }
    Serial.print("ID "); Serial.println(id);
    p = -1;
    Serial.println("Place same finger again");
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
  
    // OK success!
  
    p = finger.image2Tz(2);
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image converted");
        break;
      case FINGERPRINT_IMAGEMESS:
        Serial.println("Image too messy");
        return p;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        return p;
      case FINGERPRINT_FEATUREFAIL:
        Serial.println("Could not find fingerprint features");
        return p;
      case FINGERPRINT_INVALIDIMAGE:
        Serial.println("Could not find fingerprint features");
        return p;
      default:
        Serial.println("Unknown error");
        return p;
    }
  
    // OK converted!
    Serial.print("Creating model for #");  Serial.println(id);
  
    p = finger.createModel();
    if (p == FINGERPRINT_OK) {
      Serial.println("Prints matched!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_ENROLLMISMATCH) {
      Serial.println("Fingerprints did not match");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  
    Serial.print("ID "); Serial.println(id);
    p = finger.storeModel(id);
    if (p == FINGERPRINT_OK) {
      Serial.println("Stored!");
    } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
      Serial.println("Communication error");
      return p;
    } else if (p == FINGERPRINT_BADLOCATION) {
      Serial.println("Could not store in that location");
      return p;
    } else if (p == FINGERPRINT_FLASHERR) {
      Serial.println("Error writing to flash");
      return p;
    } else {
      Serial.println("Unknown error");
      return p;
    }
  
    return true;
}
