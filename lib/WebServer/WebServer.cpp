#include "SerialLogger.h"
#include "WebServer.h"
#include <AsyncElegantOTA.h>
#include <WebSerial.h>

bool WebServer::available = false;

WebServer::WebServer()
{
    this->server = new AsyncWebServer(WEB_SERVER_PORT);
    // this->ws = new AsyncWebSocket("/ws");
}

WebServer::WebServer(int port)
{
    this->server = new AsyncWebServer(port);
    // this->ws = new AsyncWebSocket("/ws");
}

void WebServer::socketHandler(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
                              void *arg, uint8_t *data, size_t len)
{
    switch (type)
    {
    case WS_EVT_CONNECT:
        Logger.Info("WebSocket client " + String(client->id()) + " connected from " + client->remoteIP().toString().c_str());
        break;
    case WS_EVT_DISCONNECT:
        Logger.Warning("WebSocket client " + String(client->id()) + " disconnected ");
        break;
    case WS_EVT_DATA:
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
        {
            data[len] = 0;
            String msg = (char *)data;
            Logger.Info("WS received: " + msg);
        }
        break;
    }
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
        Logger.Error("WS: Error event");
        break;
    }
}

void WebServer::start(bool enableOTA, void (*callback)(void))
{
    WebSerial.begin(this->server);
    // this->ws->onEvent(this->socketHandler);
    this->server->addHandler(this->ws);
    if (enableOTA)
    {
        this->enableOTA();
    }
    this->server->on("/", HTTP_GET, WebServer::indexRoute);
    this->server->onNotFound(WebServer::notFoundRoute);
    this->server->begin();
    WebServer::available = true;
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