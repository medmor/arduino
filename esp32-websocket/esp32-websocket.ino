#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "your_SSID";
const char *password = "your_PASSWORD";

IPAddress staticIP(192, 168, 1, 20);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
    if (type == WS_EVT_CONNECT)
    {
        Serial.println("WebSocket client connected");
    }
    else if (type == WS_EVT_DISCONNECT)
    {
        Serial.println("WebSocket client disconnected");
    }
    else if (type == WS_EVT_DATA)
    {
        Serial.println("WebSocket data received");
        // Echo the data back to the client
        client->text(String((char *)data, len));
    }
}

void setup()
{
    Serial.begin(115200);

    WiFi.config(staticIP, gateway, subnet, dns);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println("Connected to WiFi");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "text/plain", "Hello, world!"); });

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);
    server.begin();
}

void loop()
{
    // Nothing needed here with AsyncWebServer
}