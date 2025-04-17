#include <my_firebase.h>

#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"  //Provide the RTDB payload printing info and other helper functions.

static const char *TAG = "FIREBASE";

void firebaseInit(void) {
  config.api_key = API_KEY;               /* Assign the api key (required) */
  config.database_url = DATABASE_URL;     /* Assign the RTDB URL (required) */

  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // Reconnect Firebase when lost connection
  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);
    
    // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  config.max_token_generation_retry = 5;    // Assign the maximum retry of token generation
  Firebase.begin(&config, &auth);           // Khởi tạo firebase

  // Lấy thông tin UserId - Sẽ mất 1 vài giây
  while ((auth.token.uid) == "") {
    ESP_LOGD(TAG, ".");
    delay(500);
  }

  // 
  uid = auth.token.uid.c_str();
}

bool fbGetSignalAddUser(uint8_t *registerId) {
  // Path đăng ký
  String path = "Store/";
  path.concat(uid);
  path.concat("/register");

  // Lấy dữ liệu JSON từ firebase
  if(Firebase.RTDB.getJSON(&fbdo, path, &json)) {
    String jsonString = fbdo.to<FirebaseJson>().raw();
    
    // Giải mã
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    // Đọc về empty cũng sẽ báo error
    if(error) {
      ESP_LOGD(TAG, "ERR: %s", error.f_str());
      return false;
    }

    int id = doc["uid"];      // fingerID dùng để đăng ký
    bool sta = doc["status"]; // trạng thái đăng ký
    *registerId = id;

    return sta ? true : false;
  }
  else return false;
}

void fbSendtring(String pathMain, String pathRef, String content) {
  // Path 
  String path = pathMain;
  path.concat(uid);
  path.concat(pathRef);

  // ESP_LOGD(TAG, "%s", path.c_str());

  Firebase.RTDB.setString(&fbdo, path.c_str(), content.c_str());
}

void fbSendBoolean(String pathMain, String pathRef, bool state) {
    // Path 
    String path = pathMain;
    path.concat(uid);
    path.concat(pathRef);
  
    // ESP_LOGD(TAG, "%s", path.c_str());
  
    Firebase.RTDB.setBool(&fbdo, path.c_str(), state);
}