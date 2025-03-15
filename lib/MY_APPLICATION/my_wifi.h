#ifndef MY_WIFI
#define MY_WIFI

// Thư viện kết nối wifi
#include <WiFi.h>
const char* ssid = "by GT";
const char* password = "abcxyzhehe";

void wifi_init(void) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("WiFi đang kết nối ..");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print('.');
    }
    Serial.println("WiFi đã kết nối thành công");
    Serial.println(WiFi.localIP());
  }
   
void wifi_smart_config(void) {
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();  /* start SmartConfig */
  
    /* Wait for SmartConfig packet from mobile */
    Serial.println("WiFi SmartConfig đang kết nối ....");  
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi SmartConfig kết nối thành công.");
   
    /* Wait for WiFi to connect to AP */
    Serial.println("WiFi đang kết nối");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi kết nối thành công. ");
    Serial.print("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
  }

  
#endif