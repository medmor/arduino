#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

class CarServer
{
public:
    CarServer(int sPort, int wPort);
    ~CarServer();
    void setup();
    void loop();

private:
    int serverPort;
    int webSocketPort;
    ESP8266WebServer server = ESP8266WebServer(serverPort);
    WebSocketsServer webSocket = WebSocketsServer(webSocketPort);

    void handleIndex();
};
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
