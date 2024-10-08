#include <Arduino.h>

#include <wifi.h>

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
