#include <Arduino.h>
#include "net_config.h"
#include "car_server.h"

CarServer server;

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("Starting...");

    begin_access_point();

    server.setup();
}

void loop()
{
    server.loop();
}