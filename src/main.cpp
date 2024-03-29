#include <Arduino.h>
//#include <DiOremote.h>
//#include <EEPROM.h>
//#include <Oregon.h>
//#include <PubSubClient.h>
#include <Scheduler.h>
//#include "../lib/WebServer/WebServer.h"
//#include <IRremote.h>
//#include "IRManager.h"
#include "common_config.h"
// #include "SerialLogger.h"
//#include "WiFiManager.h"
#include <IRremote.h>
#include "BlinkTask/BlinkTask.h"
#include "MainTask/MainTask.h"


//IRAM_ATTR void ext_int_1(); // DOT NOT REMOVE
//ICACHE_RAM_ATTR void ext_int_1(); // DOT NOT REMOVE

//int temp = 0
//long lastMsg = 0;
//long lastLog = 0;
//
//char message_buff[100];
//
//const unsigned long ON_CODE = 1278825104;
//const unsigned long OFF_CODE = 1278825088;
//
//WiFiManager *wifiManager;
//WiFiClient wifiClient;
//
//PubSubClient mqttClient(MQTT_HOST, MQTT_PORT, wifiClient);
//DiOremote myRemote = DiOremote(ESP_EMIT_PIN);
//
//IRManager *IR;

//void startWebServer()
//{
//  WebServer *server = new WebServer();
//  server->start(true, [](uint8_t *data, size_t len)
//                { Logger.Info("CALLBACK called"); });
//}
//
//void startIRManager()
//{
//  IR = new IRManager();
//}
//
//void listen2IR()
//{
//  IR->listen();
//}
//
//void sendIR()
//{
//  IR->send();
//}

//void listen2RF()
//{
//  long now = millis();
//  cli();
//  word p = pulse;
//  pulse = 0;
//  sei();
//
//  if (p != 0)
//  {
//    if (orscV2.nextPulse(p))
//    {
//      const byte *DataDecoded = DataToDecoder(orscV2);
//      byte source = channel(DataDecoded);
//      const char* type = OregonType(DataDecoded);
//      temp = temperature(DataDecoded);
//      int batt = battery(DataDecoded);
//      int hum = humidity(DataDecoded);
//      String tempStr = "";
//      if (strcmp(OREGON_TYPE, type) == 0 && source == 1 && temp < 30 && temp > 0 && batt > 0)
//      {
//        String json = "{\"source\":";
//        json += String(source);
//        json += ",\"temperature\":";
//        json += temperature(DataDecoded);
//        json += ",\"humidity\":";
//        json += humidity(DataDecoded);
//        json += ",\"battery\":\"";
//        json += battery(DataDecoded) > 50 ? "ok" : "low";
//        json += "\"}";
//        tempStr += temperature(DataDecoded);
//        Logger.Debug(json);
//        if (now - lastMsg > 1000 * 60)
//        {
//          lastMsg = now;
//        }
//        mqttClient.publish(TEMP_TOPIC, tempStr.c_str(), true);
//        mqttClient.publish(HUM_TOPIC, String(hum).c_str(), true);
//      }
//    }
//  }
//  if (now - lastLog > 1000 * 60)
//  {
//    lastLog = now;
//    mqttClient.publish(LOG_TOPIC, "Running.. System seems ok.", true);
//  }
//}

//void routine()
//{
//  listen2RF();
//  listen2IR();
//}

//void setDioRelay(bool val)
//{
//  val ? myRemote.send(ON_CODE) : myRemote.send(OFF_CODE);
//}
//
//void parseData(String msg)
//{
//  Logger.Debug("PARSING MESSAGE:");
//  Logger.Debug(msg);
//  if (msg.indexOf(",") != msg.length())
//  {
//    String prop1 = msg.substring(1, msg.indexOf(",") - 1);
//    String prop2 = msg.substring(prop1.length() + 2, msg.length() - 2);
//    prop1 = prop1.substring(1, prop1.length() - 2);
//    prop2 = prop2.substring(1, prop2.length() - 2);
//    String val1 = prop1.substring(prop1.indexOf(":") + 2, prop1.length());
//    prop1 = prop1.substring(0, prop1.indexOf(":") - 1);
//
//    String val2 = prop2.substring(prop2.indexOf(":") + 2, prop2.length() - 1);
//    val2 = val2.substring(0, val2.length() - 1);
//    prop2 = prop2.substring(0, prop2.indexOf(":") - 1);
//
//    int size = 2;
//    String props[size] = {prop1, prop2};
//    String values[size] = {val1, val2};
//
//    for (int i = 0; i < size; i++)
//    {
//      if (props[i] == "state")
//      {
//        bool val = values[i] == "true" ? true : false;
//        setDioRelay(val);
//      }
//    }
//  }
//  else
//  {
//    String propName = msg.substring(2, msg.indexOf(":") - 2);
//    String propVal = msg.substring(msg.indexOf(":") + 2, msg.length() - 2);
//    propName = propName.substring(0, propName.length() - 1);
//    propVal = propVal.substring(0, propVal.length() - 2);
//    Logger.Debug("PROP ELSE WIP:");
//    Logger.Debug(propName);
//    Logger.Debug(propVal);
//  }
//}
//
//void sendIP()
//{
//  mqttClient.publish(GIVE_IP, WiFi.localIP().toString().c_str(), true);
//}

//void mqttCallback(char *tpc, byte *payload, unsigned int length)
//{
//  unsigned int i = 0;
//  String topic = String(tpc);
//  Logger.Info("Message received on topic: " + topic);
//  // create character buffer with ending null terminator (string)
//  for (i = 0; i < length; i++)
//  {
//    message_buff[i] = payload[i];
//  }
//  message_buff[i] = '\0';
//
//  String msgString = String(message_buff);
//  Logger.Info(msgString);
//  if (topic == RELAY_SET_TOPIC)
//  {
//    parseData(msgString);
//  }
//  else if (topic == ASK_IP)
//  {
//    sendIP();
//  }
//  else if (topic == AMPLI_SET_TOPIC)
//  {
//    sendIR();
//  }
//}

void setup()
{
  Serial.begin(SERIAL_LOGGER_BAUD_RATE);
  //attachInterrupt(digitalPinToInterrupt(ESP_RECV_PIN), ext_int_1, CHANGE);

  BlinkTask* ledTask = BlinkTask::getInstance();
  MainTask* mainTask = MainTask::getInstance();

  Scheduler.start(ledTask);
  Scheduler.start(mainTask);
  
  Scheduler.begin();

  /*attachInterrupt(digitalPinToInterrupt(ESP_RECV_PIN), ext_int_1, CHANGE);
  wifiManager = WiFiManager::getInstance();
  wifiManager->config(_SSID, WPA_KEY);
  if (wifiManager->connect())
  {
    SerialLogger::initializeTime();
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
    {
      mqttClient.setCallback(mqttCallback);
      mqttClient.subscribe(RELAY_SET_TOPIC);
      mqttClient.subscribe(ASK_IP);
      mqttClient.subscribe(AMPLI_SET_TOPIC);
      Logger.Info("MQTT broker connected !");
    }
    else
    {
      Logger.Error("while connecting to MQTT broker");
    }
  }
  startIRManager();
  startWebServer();*/
}

void loop()
{
  //mqttClient.loop();
  //routine();
}