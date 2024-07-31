#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer("/ws");

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] Received text: %s\n", num, payload);

        String echoMessage = "Received:  " + String((char *)payload);
        webSocket.sendTXT(num, echoMessage);
        break;
    }
}

void server_setup()
{

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    server.on("/", HTTP_GET, []()
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
    } });

    server.begin();
    Serial.print("ESP8266 Web Server's IP address: ");
    Serial.println(WiFi.localIP());
}

void server_loop()
{
    server.handleClient();

    webSocket.loop();
}
