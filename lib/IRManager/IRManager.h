#ifndef IR_MANAGER_H
#define IR_MANAGER_H
#define USE_IRREMOTE_HPP_AS_PLAIN_INCLUDE // prevent IRRemote multiple definitions linker error

#include "../../include/common_config.h"
#include <IRremoteInt.h>

class IRManager
{
private:
public:
    IRManager();
    void listen();
    void send();
};

#endif