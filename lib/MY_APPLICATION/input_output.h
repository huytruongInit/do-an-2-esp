#ifndef MY_INPUT_OUTPUT
#define MY_INPUT_OUTPUT

#include <Arduino.h>

// define chân I/O (số chân đang được định nghĩa tạm thời)
#define INPUT_BUTTON_RESET 0
#define INPUT_BUTTON_MODE  0
#define INPUT_SOLENOID     0  // chân detect tín hiệu của solenoid
#define OUTPUT_LED_BICOLOR 0
#define OUTPUT_BUZZER      0
#define OUPUT_SOLENOID     0


// ----------------------------------------------------------------------------------- //
// ----------------------------------- PUBLIC ---------------------------------------- //
void io_init(void);

#endif