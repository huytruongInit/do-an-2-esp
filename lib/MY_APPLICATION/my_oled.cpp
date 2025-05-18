#include <my_oled.h>
static const char *TAG = "OLED";

void oledInit(void) {
  if(!display.begin(SCREEN_ADDRESS, true)) { 
      ESP_LOGD(TAG, "Connection failed");
      return;
  }
  display.display();
  ESP_LOGD(TAG, "Connection success");
  display.clearDisplay();
}

// Display WiFi
void oledWiFiStart(void) {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(1);        // Draw white text
  display.setCursor(0, 0);             // Start at top-left corner
  display.print(F("Connecting to \n"));
  display.setCursor(0, 50);   
  display.setTextSize(2);          
  display.print("byGT");
  display.drawBitmap( 73, 10, Wifi_start_bits, Wifi_start_width, Wifi_start_height, 1);
  display.display();
}

void oledWiFiConnected(void) {
  display.clearDisplay();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(1);        // Draw white text
  display.setCursor(8, 0);             // Start at top-left corner
  display.print(F("Connected \n"));
  display.drawBitmap( 33, 15, Wifi_connected_bits, Wifi_connected_width, Wifi_connected_height, 1);
  display.display();
}

// Display text
void oledText(String content, int16_t x, int16_t y) {
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(1);            // Draw white text
  display.setCursor(x, y);             // Start at top-left corner
  display.print(F(content.c_str()));
  display.display();
}

// Display Finger
void oledFingerStart(void) {
  display.clearDisplay();
  display.drawBitmap( 32, 0, FinPr_start_bits, FinPr_start_width, FinPr_start_height, 1);
  display.display();
}

void oledFingerValid(int16_t x, int16_t y) {
  display.clearDisplay();

  // draw finger
  display.drawBitmap( x, y, FinPr_valid_bits, FinPr_valid_width, FinPr_valid_height, 1);
  display.display();
}

void oledFingerInvalid(void) {
  display.clearDisplay();
  display.drawBitmap( 34, 0, FinPr_invalid_bits, FinPr_invalid_width, FinPr_invalid_height, 1);
  display.display();
}

void oledFingerFail(void) {
  display.clearDisplay();
  display.drawBitmap( 32, 0, FinPr_failed_bits, FinPr_failed_width, FinPr_failed_height, 1);
  display.display();
}

void oledFingerScan(int16_t x, int16_t y) {
  display.clearDisplay();
  
  // draw finger
  display.drawBitmap( 50, 5, FinPr_scan_bits, FinPr_scan_width, FinPr_scan_height, 1);
  display.display();
}

// Display process bar
void oledProcessBar(int8_t progress, int8_t x, int8_t y, int8_t height) {
  display.clearDisplay();

  int barWidth = SCREEN_WIDTH - 2 * x;

  // Vẽ khung thanh tiến trình
  display.drawRect(x, y, barWidth, height, SH110X_WHITE);

  // Tính toán chiều rộng phần đã hoàn thành
  int filledWidth = map(progress, 0, 100, 0, barWidth - 2); // Trừ 2 để trừ viền

  // Vẽ phần đã hoàn thành
  display.fillRect(x + 1, y + 1, filledWidth, height - 2, SH110X_WHITE);

  // Hiển thị phần trăm tiến trình
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(SCREEN_WIDTH / 2 - (String(progress).length() * 6), y + height + 5); // Ước tính chiều rộng chữ
  display.print(progress);
  display.print("%");
  display.display();

}

// 
void oledClockCircle(void) {
  // get local time
  struct tm timeinfo;
  getLocalTime(&timeinfo);


  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(1);            // Draw white text
  display.setCursor(40, 5);             // Start at top-left corner
  String timedays = "Saturday";
  display.print(F(timedays.c_str()));

  display.setTextSize(3);             // Normal 1:1 pixel scale
  display.setTextColor(1);            // Draw white text
  display.setCursor(20, 30);             // Start at top-left corner

  String hours = String(timeinfo.tm_hour);
  String mins = String(timeinfo.tm_min);
  // String secs = String(timeinfo.tm_sec);

  // String timehms = "21:19:30";
  if( mins.length() < 2) 
    mins = "0" + mins;

  String timehms = hours + ":" + mins;

  display.print(F(timehms.c_str()));

  display.display();

}

// Display clear
void oledClearAll(void) {
  display.clearDisplay();
}