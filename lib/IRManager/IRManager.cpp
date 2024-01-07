#include "IRManager.h"
#include "SerialLogger.h"

#include "../../include/common_config.h"
#include <IRremoteInt.h>

IRManager::IRManager()
{
    Logger.Info("Hello from IRManager");
    IrReceiver.begin(IR_RECV_PIN, false);
    IrSender.begin(false);
}

void IRManager::listen()
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.command == 31)
        {
            Logger.Debug("RECEIVED COMMAND");
            Serial.println(IrReceiver.decodedIRData.address);
            Serial.println(IrReceiver.decodedIRData.numberOfBits);
            Serial.println(IrReceiver.decodedIRData.command);
        }
    }
    IrReceiver.resume();
}

void IRManager::send()
{
    Logger.Debug("IR: Sending ampli command");
    IrSender.enableIROut(38);
    IrSender.sendNEC(122, 31, 1);
    Logger.Debug("IR: Send end");
}