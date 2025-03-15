#include <secret.h>

// Thư viện kết nối wifi
#include <WiFi.h>
const char* ssid = "by GT";
const char* password = "abcxyzhehe";

// Thư viện kết nối firebase
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h" //Provide the token generation process info.
#include "addons/RTDBHelper.h"  //Provide the RTDB payload printing info and other helper functions.

FirebaseData fbdo;  // Define Firebase Data object
FirebaseAuth auth;
FirebaseConfig config;


// Thư viện kết nối vân tay
/**
 *  VCC: 3.3V 
 *  GND: GND
 *  TX:  RX
 *  RX:  TX 
*/
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

HardwareSerial mySerial(0); // sử dụng uart0 (hardware)
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Thư viện kết nối rfid
/**
 *  SDA:    5
 *  SCK:    18
 *  MOSI:   23
 *  MISO:   19
 *  IRQ:    NC 
 *  GND:    GND
 *  RST:    21
 *  VCC:    3.3V
*/
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin}; // Create SPI driver
MFRC522 mfrc522{driver};         // Create MFRC522 instance

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

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)

// define chân I/O (số chân đang được định nghĩa tạm thời)
#define INPUT_BUTTON_RESET 0
#define INPUT_BUTTON_MODE  0
#define INPUT_SOLENOID     0  // chân detect tín hiệu của solenoid
#define OUTPUT_LED_BICOLOR 0
#define OUTPUT_BUZZER      0
#define OUPUT_SOLENOID     0

void io_init(void);

// function declaration
void wifi_init(void);
void wifi_smart_config(void);
void firebase_init(void);
void finger_init(void);
void rfid_init(void);
void oled_init(void);

void setup() {
  Serial.begin(57600);
  io_init();
  // wifi_init();
  wifi_smart_config();
  firebase_init();
  finger_init();
  // rfid_init();
  // oled_init();
}

void loop() {


}
// ----------------------------------------------------------------------------------------------------- // 
// ------------------------------------------ WiFi ------------------------------------------------- // 
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

// ----------------------------------------------------------------------------------------------------- // 
// ------------------------------------------ Firebase ------------------------------------------------- // 
void firebase_init(void) {
  config.api_key = API_KEY; /* Assign the api key (required) */
  config.database_url = DATABASE_URL; /* Assign the RTDB URL (required) */
}

// ----------------------------------------------------------------------------------------------------- // 
// ------------------------------------------ Finger ------------------------------------------------- //
void finger_init(void) {
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Vân tay kết nối thành công");
  } else {
    Serial.println("Vân tay kết nối thất bại :(");
    while (1) { delay(1); } // CẦN FIX LẠI CHỔ NÀY
  }
}

// ----------------------------------------------------------------------------------------------------- // 
// ------------------------------------------ RFID ------------------------------------------------- //
void rfid_init(void) {
  mfrc522.PCD_Init();    // Init MFRC522 board.
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);	// Show details of PCD - MFRC522 Card Reader details.
  Serial.println("RFID đã kết nối thành công"); // CẦN FIX LẠI CHỔ NÀY
}

// ----------------------------------------------------------------------------------------------------- // 
// ------------------------------------------ OLED ------------------------------------------------- //
void oled_init(void) {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("OLED kết nối thất bại :("));
    for(;;);  // CẦN FIX LẠI CHỔ NÀY
  }
  delay(2000);
  display.clearDisplay();
  Serial.println("OLED đã kết nối thành công");
}

/**
 * Note: 
  Nếu tín hiệu cảm biến giữa chừng không nhận nữa thì phải làm sao để detect là cảm biến không còn kết nối ? Watch dog timer
  Chưa test thử nếu có 2 app, hoặc 2 con esp nằm gần nhau thì sẽ kết nối như thế nào (smart config)
*/

void io_init(void) {
  pinMode(INPUT_BUTTON_RESET, INPUT);
  pinMode(INPUT_BUTTON_MODE, INPUT);
  pinMode(INPUT_SOLENOID, INPUT);
  pinMode(OUTPUT_LED_BICOLOR, OUTPUT);
  pinMode(OUTPUT_BUZZER, OUTPUT);
  pinMode(OUPUT_SOLENOID, OUTPUT);
}