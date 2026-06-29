#include <ESP8266WebServer.h>
#include <car.h>

class CarServer
{
public:
    void setup();
    void loop();

private:
    void handle_index();
    void handle_connect();
    void handle_forward();
    void handle_backward();
    void handle_left();
    void handle_right();
    void handle_stop();
    void handle_NotFound();

    void checkClientConnection();

    ESP8266WebServer server = ESP8266WebServer(80);
    boolean is_connected = false;
    unsigned long lastHeartbeat = 0;
    const unsigned long heartbeatTimeout = 1000;
    Car car = Car(D7, D1, D2, D5, D3, D4);
};
