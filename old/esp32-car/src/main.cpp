#include <Arduino.h>

#include <wifiSetup.h>

#include <car_server.h>

CarServer server;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  begin_access_point();

  server.setup();
}

void loop()
{
  server.loop();
}
