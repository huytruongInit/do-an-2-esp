#include <input_output.h>

void io_init(void) {
    pinMode(INPUT_BUTTON_RESET, INPUT);
    pinMode(INPUT_BUTTON_MODE, INPUT);
    pinMode(INPUT_SOLENOID, INPUT);
    pinMode(OUTPUT_LED_BICOLOR, OUTPUT);
    pinMode(OUTPUT_BUZZER, OUTPUT);
    pinMode(OUPUT_SOLENOID, OUTPUT);
}
