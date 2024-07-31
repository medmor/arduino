#include "car_server.h"
#include <LittleFS.h>

void CarServer::setup()
{
    LittleFS.begin();
    webSocket.begin();
    webSocket.onEvent([this](uint8_t num, WStype_t type, unsigned char *payload, size_t length)
                      { webSocketEvent(num, type, payload, length); });

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
