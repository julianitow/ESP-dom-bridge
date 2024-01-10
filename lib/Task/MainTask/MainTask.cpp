#include "MainTask.h"
#include "SerialLogger.h"

#include "BlinkTask/BlinkTask.h"

#include <EEPROM.h>//Mandatory to be placed here (multiple definitions error)
#include <Oregon.h>//Mandatory to be placed here (multiple definitions error)

IRAM_ATTR void ext_int_1(); // DOT NOT REMOVE

MainTask* MainTask::_instance;
OregonData MainTask::lastData;

MainTask* MainTask::getInstance() {
    if (MainTask::_instance == nullptr) {
        MainTask::_instance = new MainTask();
    }
    return MainTask::_instance;
}

MainTask::MainTask() {
    this->IR = new IRManager();
    this->mqttClient = PubSubClient(MQTT_HOST, MQTT_PORT, wifiClient);
    //DiOremote remote = DiOremote(ESP_EMIT_PIN);
    //this->myRemote = DiOremote(ESP_EMIT_PIN);
}

void MainTask::setup() {
    String helloStr = "Hello from ESP Bridge v" + String(FIRMWARE_VERSION);
    attachInterrupt(digitalPinToInterrupt(ESP_RECV_PIN), ext_int_1, CHANGE);
    Logger.Info(helloStr);
    wifiManager = WiFiManager::getInstance();
    wifiManager->config(_SSID, WPA_KEY);

    if (wifiManager->connect())
    {
        SerialLogger::initializeTime();
        if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD))
        {
            mqttClient.setCallback(MainTask::mqttCallback);
            mqttClient.subscribe(RELAY_SET_TOPIC);
            mqttClient.subscribe(ASK_IP);
            mqttClient.subscribe(AMPLI_SET_TOPIC);
            Logger.Info("MQTT broker connected !");
            BlinkTask::getInstance()->status = STAY;
        }
        else
        {
            Logger.Error("while connecting to MQTT broker");
            BlinkTask::getInstance()->status = ERROR;
        }
        this->startWebServer();
    } else {
        BlinkTask::getInstance()->status = ERROR;
    }
}

void MainTask::loop() {
    this->mqttClient.loop();
    this->IR->listen();
    this->listen2RF();
}

void MainTask::listen2RF() {
    long now = millis();
    cli();
    word p = pulse;
    pulse = 0;
    sei();

    if (p != 0)
    {
        if (orscV2.nextPulse(p))
        {
            const byte *DataDecoded = DataToDecoder(orscV2);
            byte source = channel(DataDecoded);
            const char* type = OregonType(DataDecoded);
            temp = temperature(DataDecoded);
            int batt = battery(DataDecoded);
            int hum = humidity(DataDecoded);
            String tempStr = "";
            if (strcmp(OREGON_TYPE, type) == 0 && source == 1 && temp < 30 && temp > 0 && batt > 0)
            {
                BlinkTask::getInstance()->status = BLINK_FAST;
                String json = "{\"source\":";
                json += String(source);
                json += ",\"temperature\":";
                json += temperature(DataDecoded);
                json += ",\"humidity\":";
                json += humidity(DataDecoded);
                json += ",\"battery\":\"";
                json += battery(DataDecoded) > 50 ? "ok" : "low";
                json += "\"}";
            tempStr += temperature(DataDecoded);
                Logger.Debug(json);
                if (now - lastMsg > 1000 * 60)
                {
                lastMsg = now;
                }
                mqttClient.publish(TEMP_TOPIC, tempStr.c_str(), true);
                mqttClient.publish(HUM_TOPIC, String(hum).c_str(), true);

            }
        }
    }
    if (now - lastLog > 1000 * 60)
    {
        lastLog = now;
        mqttClient.publish(LOG_TOPIC, "Running.. System seems ok.", true);
        BlinkTask::getInstance()->status = BLINK_FAST;
    }
    BlinkTask::getInstance()->status = STAY;
}

void MainTask::setDioRelay(bool val)
{
  // val ? myRemote.send(ON_CODE) : myRemote.send(OFF_CODE);
}

void MainTask::parseData(String msg) {
  Logger.Debug("PARSING MESSAGE:");
  Logger.Debug(msg);
  if (msg.indexOf(",") != msg.length())
  {
    String prop1 = msg.substring(1, msg.indexOf(",") - 1);
    String prop2 = msg.substring(prop1.length() + 2, msg.length() - 2);
    prop1 = prop1.substring(1, prop1.length() - 2);
    prop2 = prop2.substring(1, prop2.length() - 2);
    String val1 = prop1.substring(prop1.indexOf(":") + 2, prop1.length());
    prop1 = prop1.substring(0, prop1.indexOf(":") - 1);

    String val2 = prop2.substring(prop2.indexOf(":") + 2, prop2.length() - 1);
    val2 = val2.substring(0, val2.length() - 1);
    prop2 = prop2.substring(0, prop2.indexOf(":") - 1);

    int size = 2;
    String props[size] = {prop1, prop2};
    String values[size] = {val1, val2};

    for (int i = 0; i < size; i++)
    {
      if (props[i] == "state")
      {
        bool val = values[i] == "true" ? true : false;
        setDioRelay(val);
      }
    }
  }
  else
  {
    String propName = msg.substring(2, msg.indexOf(":") - 2);
    String propVal = msg.substring(msg.indexOf(":") + 2, msg.length() - 2);
    propName = propName.substring(0, propName.length() - 1);
    propVal = propVal.substring(0, propVal.length() - 2);
    Logger.Debug("PROP ELSE WIP:");
    Logger.Debug(propName);
    Logger.Debug(propVal);
  }
}

void MainTask::mqttCallback(char *tpc, byte *payload, unsigned int length) {
    BlinkTask::getInstance()->status = BLINK_FAST;
    char message_buff[100];
    unsigned int i = 0;
    String topic = String(tpc);
    Logger.Info("Message received on topic: " + topic);
    // create character buffer with ending null terminator (string)
    for (i = 0; i < length; i++)
    {
        message_buff[i] = payload[i];
    }
    message_buff[i] = '\0';
    String msgString = String(message_buff);
    Logger.Info(msgString);
    if (topic == RELAY_SET_TOPIC)
    {
        parseData(msgString);
    }
    else if (topic == ASK_IP)
    {
        MainTask::getInstance()->mqttClient.publish(GIVE_IP, WiFi.localIP().toString().c_str(), true);
    }
    else if (topic == AMPLI_SET_TOPIC)
    {
        MainTask::getInstance()->IR->send();
    }
    BlinkTask::getInstance()->status = STAY;
}

void MainTask::startWebServer() {
    WebServer *server = new WebServer();
    server->start(true, [](uint8_t *data, size_t len)
                    { Logger.Info("CALLBACK called"); });
}