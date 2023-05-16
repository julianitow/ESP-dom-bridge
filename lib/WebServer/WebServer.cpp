#include "SerialLogger.h"
#include "WebServer.h"
#include <AsyncElegantOTA.h>

WebServer::WebServer()
{
    this->server = new AsyncWebServer(WEB_SERVER_PORT);
    this->ws = new AsyncWebSocket("/ws");
}

WebServer::WebServer(int port)
{
    this->server = new AsyncWebServer(port);
    this->ws = new AsyncWebSocket("/ws");
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
    this->ws->onEvent(this->socketHandler);
    this->server->addHandler(this->ws);
    if (enableOTA)
    {
        this->enableOTA();
    }
    this->server->on("/", HTTP_GET, WebServer::indexRoute);
    this->server->on("/logs", HTTP_GET, WebServer::loggingRoute);
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

void WebServer::loggingRoute(AsyncWebServerRequest *request)
{
    // TODO MOVE IT FROM HERE
    const char *loggingHtml = R"rawliteral(
        <!DOCTYPE HTML><html>
        <head>
        <title>ESP Web Logging</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        <style>
            .content {
                padding: 30px;
                max-width: 600px;
                margin: 0 auto;
            }
        </style>
        <title>ESP Web Logging</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="icon" href="data:,">
        </head>
        <body>
        <div>
            <h1>ESP WebSocket Loging</h1>
        </div>
        <div class="content">

        </div>
        <script>
        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;
        window.addEventListener('load', onLoad);
        function initWebSocket() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onopen    = onOpen;
            websocket.onclose   = onClose;
            websocket.onmessage = onMessage;
        }
        function onOpen(event) {
            console.log('Connection opened');
        }
        function onClose(event) {
            console.log('Connection closed');
            setTimeout(initWebSocket, 2000);
        }
        function onMessage(event) {
            console.log(event);
        }
        function onLoad(event) {
            initWebSocket();
        }
        </script>
        </body>
        </html>)rawliteral";

    request->send(200, "text/html", loggingHtml);
}
