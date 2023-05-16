#include "WebServer.h"
#include <AsyncElegantOTA.h>

WebServer::WebServer()
{
    this->server = new AsyncWebServer(WEB_SERVER_PORT);
}

WebServer::WebServer(int port)
{
    this->server = new AsyncWebServer(port);
}

void WebServer::start(bool enableOTA)
{
    if (enableOTA)
    {
        this->enableOTA();
    }
    this->server->on("/", HTTP_GET, WebServer::indexRoute);
    this->server->onNotFound(WebServer::notFoundRoute);
    this->server->begin();
}

void WebServer::enableOTA()
{
    AsyncElegantOTA.begin(this->server);
}

void WebServer::notFoundRoute(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void WebServer::indexRoute(AsyncWebServerRequest *request)
{
    String index = "INDEX PAGE TO DO";
    request->send(200, "text/plain", index);
}
