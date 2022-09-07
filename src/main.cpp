#include <Arduino.h>
#include <DiOremote.h>
#include <EEPROM.h>
#include <Oregon.h>
#include <PubSubClient.h>

#include "../include/common_config.h"

#include "SerialLogger.h"
#include "WiFiManager.h"

IRAM_ATTR void ext_int_1();
void mqttCallback(char* tpc, byte* payload, unsigned int length);

int temp = 0;
long lastMsg = 0;

char message_buff[100];

const unsigned long ON_CODE  = 1278825104;
const unsigned long OFF_CODE = 1278825088;

WiFiManager* wifiManager;
WiFiClient wifiClient;
PubSubClient mqttClient(MQTT_HOST, 9443, mqttCallback, wifiClient);
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
        if (now - lastMsg > 1000 * 60) {
          lastMsg = now;
        }
        mqttClient.publish(TEMP_TOPIC, String(temp).c_str(), true);
        mqttClient.publish(HUM_TOPIC, String(hum).c_str(), true);
      }
    }
  }
}

void setDioRelay(bool val) {
  val ? myRemote.send(ON_CODE) : myRemote.send(OFF_CODE);
}

void mqttCallback(char* tpc, byte* payload, unsigned int length) {
  int i = 0;
  String topic = String(tpc);
  Serial.println("Message recu =>  topic: " + topic);
  Serial.print(" | longueur: " + String(length,DEC));
  // create character buffer with ending null terminator (string)
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
  Serial.println("Payload: " + msgString);
}

void setup() {
  Serial.begin(SERIAL_LOGGER_BAUD_RATE);
  Logger.Info("Hello from ESP Bridge");
  attachInterrupt(digitalPinToInterrupt(ESP_RECV_PIN), ext_int_1, CHANGE);

  wifiManager = WiFiManager::getInstance();
  wifiManager->config("uifeedu75", "mandalorianBGdu75");
  if(wifiManager->connect()) {
    SerialLogger::initializeTime();
    if(mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
      Logger.Info("MQTT broker connected !");
    } else {
      Logger.Error("trying to connect to MQTT broker");
    }
  }
}

void loop() {
  mqttClient.loop();
  routine();
}