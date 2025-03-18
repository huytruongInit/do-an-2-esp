#ifndef MY_WIFI
#define MY_WIFI

// Thư viện kết nối wifi
#include <WiFi.h>
static const char* ssid = "by GT";
static const char* password = "abcxyzhehe";

void wifi_init(void);
void wifi_smart_config(void);

  
#endif