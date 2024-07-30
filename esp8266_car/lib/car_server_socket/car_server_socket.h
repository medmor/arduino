/*
 * This ESP8266 NodeMCU code was developed by newbiely.com
 *
 * This ESP8266 NodeMCU code is made available for public use without any restriction
 *
 * For comprehensive instructions and wiring diagrams, please visit:
 * https://newbiely.com/tutorials/esp8266/esp8266-websocket
 */
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

ESP8266WebServer server(80);                       // Web server on port 80
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket server on port 81

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
        // Send a response back to the client
        String echoMessage = "Received:  " + String((char *)payload);
        webSocket.sendTXT(num, echoMessage);
        break;
    }
}

void server_setup()
{

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    // Serve a basic HTML page with JavaScript to create the WebSocket connection
    server.on("/", HTTP_GET, []()
              {
    Serial.println("Web Server: received a web page request");
    server.send(200, "text/html", "<div>Hello from ESP8266!</div>"); });

    server.begin();
    Serial.print("ESP8266 Web Server's IP address: ");
    Serial.println(WiFi.localIP());
}

void server_loop()
{
    // Handle client requests
    server.handleClient();

    // Handle WebSocket events
    webSocket.loop();

    // TODO: Your code here
}
