#include <Arduino.h>

#include <wifi.h>
#include <car_server.h>

void setup()
{
  Serial.begin(115200);

  begin_wifi();

  setupMotores();

  serverSetup();
}

void loop()
{
  serverLoop();
  checkClientConnection();
}

#pragma endregion