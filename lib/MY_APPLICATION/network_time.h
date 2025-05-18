#include "time.h"

const char* ntpServer = "pool.ntp.org";

int timezone = 7 * 3600;
int dst = 0;

void networkTimeInit(void) {
    configTime(timezone , dst, ntpServer);
}

// Function that gets current epoch time
unsigned long getTime() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      //Serial.println("Failed to obtain time");
      return(0);
    }
    time(&now);
    return now;
}