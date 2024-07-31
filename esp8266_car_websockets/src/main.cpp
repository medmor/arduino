#include <Arduino.h>

#include <wifi.h>
// #include <car_server.h>

// CarServer server;
#include <car_server_socket.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting...");

  begin_wifi();

  // server.setup();
  server_setup();
}

void loop()
{
  // server.loop();
  server_loop();
}
