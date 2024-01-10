#ifndef BLINK_TASK_H
#define BLINK_TASK_H

#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>

#include "../../include/common_config.h"

enum led_status {
    BLINK = 0,
    BLINK_FAST = 1,
    STAY = 2,
    ERROR = 3,
};

class BlinkTask : public Task {
    
    protected:
        void setup();
        void loop();

    public:
        static BlinkTask* getInstance();
        led_status status;

    private:
        BlinkTask();

        static BlinkTask* _instance;

        uint8_t state;
        void blink(int _delay = 500);
        void stay();
};
#endif