#ifndef MY_FIREBASE
#define MY_FIREBASE

#include <secret.h>
#include <Firebase_ESP_Client.h>
#include <ArduinoJson.h>


#define USER_EMAIL "giangtruong1007@gmail.com"
#define USER_PASSWORD "giangtruong1007"

#define API_KEY "AIzaSyB_scuWAgYKmoBqG_33oBAWrLLSHp9sVq0"    // Insert Firebase project API Key
#define DATABASE_URL "https://digit-door-lock-default-rtdb.firebaseio.com/"  // Insert RTDB URLefine the RTDB URL */

// Define Firebase Data object
static FirebaseData fbdo;       
static FirebaseAuth auth;
static FirebaseConfig config;
static FirebaseJson json;   
static String uid;  // UID: 8pN513xWDacJtxHqzaIYJLpWrRp2

// fb: firebase

void firebaseInit(void);                                    // Khởi tạo kết nối firebase

// GET 
bool fbGetSignalAddUser(uint8_t *registerId);      // Đọc xem có tín hiệu đăng ký không

// SEND
void fbSendtring(String pathMain,String pathRef, String content);      // Gửi chuỗi lên firebase
void fbSendBoolean(String pathMain, String pathRef, bool state);
void fbSendInt(String pathMain,String pathRef, int value);  
#endif

