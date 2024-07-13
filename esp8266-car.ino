#include <ESP8266WebServer.h>
#include "./wifi.h"

ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);

  digitalWrite(LED_BUILTIN, HIGH);

  begin_wifi();

  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_ledon);
  server.on("/ledoff", handle_ledoff);
  server.onNotFound(handle_NotFound);

  server.begin();
}

void loop()
{
  server.handleClient();
}

void handle_OnConnect()
{
  Serial.println("LED: OFF");
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/html", updateWebpage("ON"));
}

void handle_ledon()
{
  Serial.println("LED: ON");
  digitalWrite(LED_BUILTIN, LOW);
  server.send(200, "text/html", updateWebpage("OFF"));
}

void handle_ledoff()
{
  Serial.println("LED: OFF");
  digitalWrite(LED_BUILTIN, HIGH);
  server.send(200, "text/html", updateWebpage("ON"));
}

void handle_NotFound()
{
  server.send(404, "text/plain", "Not found");
}

String updateWebpage(String LEDstatus)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html {font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #3498db;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Station(STA) Mode</h3>\n";

  if (LEDstatus == "OFF")
  {
    ptr += "<p>BLUE LED: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  }
  else
  {
    ptr += "<p>BLUE LED: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}