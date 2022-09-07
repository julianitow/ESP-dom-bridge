#include <Arduino.h>
#include <EEPROM.h>
#include <Oregon.h>

#include "WiFiManager.h"
#include "DiOremote.h"
#include "../include/common_config.h"
#include "SerialLogger.h"

IRAM_ATTR void ext_int_1();

int temp = 0;
long lastMsg = 0;

const unsigned long ON_CODE  = 1278825104;
const unsigned long OFF_CODE = 1278825088;

WiFiManager* wifiManager;
DiOremote myRemote = DiOremote(ESP_EMIT_PIN);

void routine() {
  long now = millis();
  cli();
  word p = pulse;
  pulse = 0;
  sei();
   
  if(p != 0) {
    if(orscV2.nextPulse(p)) {
      const byte* DataDecoded = DataToDecoder(orscV2); 
      byte source = channel(DataDecoded);
      int src = int(source);
      temp = temperature(DataDecoded);
      int batt = battery(DataDecoded);
      int hum = humidity(DataDecoded);
      if(source == 1 && temp < 30 && temp > 0 && batt > 50){
        String json = "{\"source\":";
        json += String(source);
        json += ",\"temperature\":";
        json += temperature(DataDecoded);
        json += ",\"humidity\":";
        json += humidity(DataDecoded);
        json += ",\"battery\":\"";
        json += battery(DataDecoded) > 50 ? "ok" : "low";
        json += "\"}";
        Logger.Debug(json);
        Serial.println("RECEIVED");
        if (now - lastMsg > 1000 * 60) {
          lastMsg = now;
        }
      }
    }
  }
}

void setDioRelay(bool val) {
  val ? myRemote.send(ON_CODE) : myRemote.send(OFF_CODE);
}

void setup() {
  Serial.begin(SERIAL_LOGGER_BAUD_RATE);
  Logger.Info("Hello from ESP Bridge");
  attachInterrupt(digitalPinToInterrupt(ESP_RECV_PIN), ext_int_1, CHANGE);

  wifiManager = WiFiManager::getInstance();
  wifiManager->config("uifeedu75", "mandalorianBGdu75");
  if(wifiManager->connect()) {
    SerialLogger::initializeTime();
  }
}

void loop() {
  routine();

}