#ifndef MAIN_TASK_H
#define MAIN_TASK_H

#include "../../include/common_config.h"

#include <Scheduler.h>
#include <Task.h>

#include <DiOremote.h>
#include <PubSubClient.h>

#include "IRManager.h"
#include "WiFiManager.h"
#include "WebServer.h"

class MainTask : public Task {

    public:
        static MainTask* getInstance();
    
    protected:
        void setup();
        void loop();

    private:
        MainTask();
        
        static MainTask* _instance;

        uint8_t state;
        WiFiManager *wifiManager;
        IRManager *IR;
        WiFiClient wifiClient;
        PubSubClient mqttClient;
        DiOremote dioRemote;

        int temp = 0;
        long lastMsg = 0;
        long lastLog = 0;

        const unsigned long ON_CODE = 1278825104;
        const unsigned long OFF_CODE = 1278825088;

        void startWebServer();
        void listen2RF();
        
        void static mqttCallback(char*, byte*, unsigned int);
        void static parseData(String);
        void static setDioRelay(bool);
};
#endif