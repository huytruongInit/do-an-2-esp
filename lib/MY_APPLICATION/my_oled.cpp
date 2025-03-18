#include <my_oled.h>

void oled_init(void) {
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("OLED kết nối thất bại :("));
      for(;;);  // CẦN FIX LẠI CHỔ NÀY
    }
    delay(2000);
    display.clearDisplay();
    Serial.println("OLED đã kết nối thành công");
}
  