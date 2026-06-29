#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <car.h>

class CarServer
{
public:
    void setup();
    void loop();

private:
    int serverPort;
    int webSocketPort;
    ESP8266WebServer server = ESP8266WebServer(80);
    WebSocketsServer webSocket = WebSocketsServer(81);
    Car car = Car(D7, D1, D2, D5, D3, D4);

    void handleIndex();
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};
