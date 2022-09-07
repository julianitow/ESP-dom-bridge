#include "WiFiManager.h"
WiFiManager* WiFiManager::instance = nullptr;
WiFiClient client;

WiFiManager::WiFiManager() {
    Logger.Info("Hello from WiFiManager");
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(WIFI_STA);
}

WiFiManager* WiFiManager::getInstance() {
    if (WiFiManager::instance == nullptr) {
        WiFiManager::instance = new WiFiManager();
    }
    return WiFiManager::instance;
}

void WiFiManager::config(String ssid, String password, int timeout) {
    this->ssid = ssid;
    this->password = password;
    this->timeout = timeout;
}

bool WiFiManager::connect() {
    if (this->ssid.length() == 0 || this->password.length() == 0) {
        Logger.Error("No SSID or Password set");
        return false;
    }
    WiFi.begin(this->ssid.c_str(), this->password.c_str());
    Logger.Info("WiFiManager: connecting to ");
    Serial.print(ssid.c_str());
    const unsigned long begin = millis();
    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        unsigned long now = millis();
        if (now - begin > timeout) {
            Logger.Error("Timeout while trying to connect to WiFi");
            return false;
        }
        delay(1000);
    }
    Serial.println();
    Logger.Info("WiFiManager: successfully connected, ip address: ");
    Serial.println(WiFi.localIP());
    return true;
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}