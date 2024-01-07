#ifndef BLINK_TASK_H
#define BLINK_TASK_H

#include <Scheduler.h>
#include <Task.h>
#include <LeanTask.h>

#include "../../include/common_config.h"

class BlinkTask : public Task {
    
    protected:
        void setup();
        void loop();

    private:
        uint8_t state;
};
#endif