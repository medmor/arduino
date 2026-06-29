#include "car_server.h"
#include <LittleFS.h>

void CarServer::setup()
{
    if (!LittleFS.begin(true))
    {
        Serial.println("Failed to mount LittleFS");
        return;
    }

    webSocket.begin();
    webSocket.onEvent([this](uint8_t num, WStype_t type, uint8_t *payload, size_t length)
                      { webSocketEvent(num, type, payload, length); });

    server.on("/", HTTP_GET, [this]()
              { handleIndex(); });

    server.begin();
    Serial.print("ESP32-C3 Web Server's IP address: ");
    Serial.println(WiFi.softAPIP());
}

void CarServer::loop()
{
    server.handleClient();
    webSocket.loop();

    if (webSocket.connectedClients() == 0)
    {
        car.stop();
    }
}

void CarServer::handleIndex()
{
    Serial.println("Web Server: received a web page request");

    File file = LittleFS.open("/index.html", "r");
    if (file)
    {
        server.streamFile(file, "text/html");
        file.close();
    }
    else
    {
        Serial.println("Failed to open file");
        server.send(404, "text/plain", "index.html not found. Did you upload LittleFS data?");
    }
}

void CarServer::webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_TEXT:
        if (String((char *)payload) == "forward")
        {
            car.forward();
        }
        else if (String((char *)payload) == "backward")
        {
            car.backward();
        }
        else if (String((char *)payload) == "left")
        {
            car.left();
        }
        else if (String((char *)payload) == "right")
        {
            car.right();
        }
        else if (String((char *)payload) == "stop")
        {
            car.stop();
        }
        break;

    default:
        break;
    }
}