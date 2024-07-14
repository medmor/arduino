#include "./wifi.h"
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
