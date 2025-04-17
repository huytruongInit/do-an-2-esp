#ifndef MY_WIFI
#define MY_WIFI

// Thư viện kết nối wifi
#include <WiFi.h>
static const char* ssid = "iPhone";
static const char* password = "abcxyzhehee";

void wifi_init(void);
void wifi_smart_config(void);

  
#endif