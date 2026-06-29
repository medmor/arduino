#include <WebServer.h>
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
    WebServer server;
    WebSocketsServer webSocket = WebSocketsServer(81);
    Car car = Car(12, 13, 14, 15);

    void handleIndex();
    void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
};
