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
void io_init(void) {
    pinMode(INPUT_BUTTON_RESET, INPUT);
    pinMode(INPUT_BUTTON_MODE, INPUT);
    pinMode(INPUT_SOLENOID, INPUT);
    pinMode(OUTPUT_LED_BICOLOR, OUTPUT);
    pinMode(OUTPUT_BUZZER, OUTPUT);
    pinMode(OUPUT_SOLENOID, OUTPUT);
}

#endif