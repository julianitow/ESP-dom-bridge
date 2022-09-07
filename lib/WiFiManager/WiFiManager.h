#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "../../include/common_config.h"
#include "SerialLogger.h"
#include <ESP8266WiFi.h>

class WiFiManager {

    private:
    String ssid;
    String password;
    int timeout;
    static WiFiManager* instance;
    WiFiManager();

    public:
    static WiFiManager* getInstance();
    WiFiClient client;
    void config(String, String, int = 10000);
    bool connect();
    bool isConnected();
};

#endif