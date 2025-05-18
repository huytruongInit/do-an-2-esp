#ifndef MY_INPUT_OUTPUT
#define MY_INPUT_OUTPUT

#include <Arduino.h>

// define chân I/O (số chân đang được định nghĩa tạm thời)
#define INPUT_BUTTON_RESET 35
#define INPUT_BUTTON_MODE  34
#define INPUT_SOLENOID     15  // chân detect tín hiệu của solenoid
#define OUTPUT_LED_RED     32  // led bicolor màu đỏ
#define OUTPUT_LED_GREEN   33  // _______________ xanh lá
#define OUTPUT_BUZZER      25
#define OUPUT_SOLENOID     27

#define IO_ENABLE   1
#define IO_DISABLE  0

// ----------------------------------------------------------------------------------- //
// ----------------------------------- PUBLIC ---------------------------------------- //
void gpioInit(void);                // Init các chân GPIO 

void ledRedEnable(bool sta);        // kích hoạt chân điều khiển led đỏ
void ledGreenEnable(bool sta);      // kích hoạt chân điều khiển led xanh

void buzzerEnable(bool sta);        // kích hoạt chân điều khiển còi

void solenoidEnable(bool sta);      // kích hoạt chân điều khiển solenoid
bool solenoidRead(void);

bool btnRead(uint8_t pin);
#endif