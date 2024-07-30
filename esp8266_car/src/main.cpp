#include <Arduino.h>

#include <wifi.h>
#include <car_server.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  begin_wifi();

  serverSetup();
}

void loop()
{
  serverLoop();
}
