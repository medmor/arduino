#include "log.h"
#include "./wifi.h"
#include "motor.h"
#include "./server.h"


void setup()
{
  logSetup();

  begin_wifi();

  setupMotores();

  serverSetup();
}

void loop()
{
  serverLoop();
}
