#include <input_output.h>

static const char *TAG = "GPIO";

void gpioInit(void) {
    pinMode(INPUT_BUTTON_RESET, INPUT);
    pinMode(INPUT_BUTTON_MODE, INPUT);
    pinMode(INPUT_SOLENOID, INPUT);

    pinMode(OUTPUT_LED_RED, OUTPUT);
    pinMode(OUTPUT_LED_GREEN, OUTPUT);

    pinMode(OUTPUT_BUZZER, OUTPUT);
    pinMode(OUPUT_SOLENOID, OUTPUT);

    tone(OUTPUT_BUZZER, 2700);
    delay(1000);
    noTone(OUTPUT_BUZZER); // Dừng phát âm thanh

    ledRedEnable(HIGH);   
    ledGreenEnable(HIGH);   
    // solenoidEnable(HIGH);


    
    ESP_LOGD(TAG, "Connection success");
}

void ledRedEnable(bool sta) {
    digitalWrite(OUTPUT_LED_RED, sta);
}

void ledGreenEnable(bool sta) {
    digitalWrite(OUTPUT_LED_GREEN, sta);
}

void buzzerEnable(bool sta) {
    digitalWrite(OUTPUT_BUZZER, sta);
}

void solenoidEnable(bool sta) {
    digitalWrite(OUPUT_SOLENOID, sta);
    ESP_LOGD(TAG, "OPEN DOOR");
}

bool solenoidRead(void) {
    return digitalRead(INPUT_SOLENOID);
}

bool btnRead(uint8_t pin) {
    return digitalRead(pin);
}