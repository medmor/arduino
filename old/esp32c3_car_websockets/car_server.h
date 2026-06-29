#include <WebServer.h>
#include <WebSocketsServer.h>
#include "car.h"

class CarServer
{
public:
    void setup();
    void loop();

private:
    WebServer server = WebServer(80);
    WebSocketsServer webSocket = WebSocketsServer(81);
    // ESP32-C3 Super Mini safe GPIOs: 2,3,4,5 (avoid 8/9 used by flash/USB).
    Car car = Car(2, 3, 4, 5);

    void handleIndex();
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};