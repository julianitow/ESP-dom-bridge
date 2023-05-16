#ifndef WEB_SERVER
#define WEB_SERVER

#include <ESPAsyncWebServer.h>
#include "../../include/common_config.h"
class WebServer
{
private:
    AsyncWebServer *server;
    void enableOTA();

public:
    WebServer();
    WebServer(int port);
    void start(bool enableOTA);
    static void notFoundRoute(AsyncWebServerRequest *request);
    static void indexRoute(AsyncWebServerRequest *request);
};
#endif