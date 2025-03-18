#ifndef MY_OLED
#define MY_OLED

// Thư viện kết nối màn hình oled
/**
 *  VCC:  3.3V
 *  GND:  GND
 *  SCL:  22
 *  SDA:  21 
*/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

void oled_init(void);

#endif