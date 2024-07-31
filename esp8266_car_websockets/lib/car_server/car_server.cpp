#include "car_server.h"
#include <LittleFS.h>

CarServer::CarServer(int sPort, int wPort)
{
    serverPort = sPort;
    webSocketPort = wPort;
}

CarServer::~CarServer()
{
}

void CarServer::setup()
{
    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    server.on("/", HTTP_GET, [this]()
              { handleIndex(); });

    server.begin();
    Serial.print("ESP8266 Web Server's IP address: ");
    Serial.println(WiFi.localIP());
}

void CarServer::loop()
{
    server.handleClient();

    webSocket.loop();
}

void CarServer::handleIndex()
{
    Serial.println("Web Server: received a web page request");
    if (!LittleFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }

    File file = LittleFS.open("/index.html", "r");
    if (file)
    {
        server.streamFile(file, "text/html");
        file.close();
    }
    else
    {
        Serial.println("Failed to open file");
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        Serial.printf("[%u] Connected from \n", num);
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] Received text: %s\n", num, payload);

        String echoMessage = "Received:  " + String((char *)payload);
        break;
    }
}
