#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include "../../include/common_config.h"
class WebServer
{
private:
    AsyncWebServer *server;
    AsyncWebSocket *ws;
    void enableOTA();
    static void socketHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                              void *arg, uint8_t *data, size_t len);
    void callback();

public:
    WebServer();
    WebServer(int port);
    void start(bool enableOTA, void (*callback)());
    static void notFoundRoute(AsyncWebServerRequest *request);
    static void indexRoute(AsyncWebServerRequest *request);
    static void loggingRoute(AsyncWebServerRequest *request);
};
#endif