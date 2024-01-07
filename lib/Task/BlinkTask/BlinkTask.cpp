#include "BlinkTask.h"

void BlinkTask::setup() {
    state = HIGH;
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, state);
}

void BlinkTask::loop() {
    state = state == HIGH ? LOW : HIGH;
    digitalWrite(LED_PIN, state);

    delay(1000);
}