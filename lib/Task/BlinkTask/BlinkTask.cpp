#include "BlinkTask.h"

BlinkTask* BlinkTask::_instance;

BlinkTask::BlinkTask() {};

BlinkTask* BlinkTask::getInstance() {
    if (BlinkTask::_instance == nullptr) {
        BlinkTask::_instance = new BlinkTask();
    }
    return BlinkTask::_instance;
}

void BlinkTask::setup() {
    state = HIGH;
    status = BLINK;
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, state);
}

void BlinkTask::loop() {
    switch (this->status)
    {
    case BLINK:
        this->blink();
        break;
    case BLINK_FAST:
        this->blink(200);
        break;
    case STAY:
        this->stay();
        break;
    case ERROR:
        this->blink(100);
        break;
    default:
        break;
    }
}

void BlinkTask::blink(int _delay) {
    state = state == HIGH ? LOW : HIGH;
    digitalWrite(LED_PIN, state);

    delay(_delay);
}

void BlinkTask::stay() {
    state = HIGH;
    digitalWrite(LED_PIN, state);
}