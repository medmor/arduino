#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <motor.h>

boolean is_connected = false;
unsigned long lastHeartbeat = 0;
const unsigned long heartbeatTimeout = 1000;

ESP8266WebServer server(80);
void serverLoop()
{
    server.handleClient();
}
void handle_connect()
{
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Connected");
    lastHeartbeat = millis();
}

void checkClientConnection()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastHeartbeat > heartbeatTimeout)
    {
        // // // Serial.println("Client disconnected");
        is_connected = false;
    }
    else
    {
        // // // Serial.println("Client connected");
        is_connected = true;
    }
    if (WiFi.status() != WL_CONNECTED || !is_connected)
    {
        motor_stop();
    }
}

void handle_forward()
{
    if (is_connected)
    {
        motor_forward();
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Forward");
    }
}

void handle_backward()
{
    if (is_connected)
    {
        motor_backward();
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Backward");
    }
}

void handle_left()
{
    if (is_connected)
    {
        motor_left();
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Left");
    }
}

void handle_right()
{
    if (is_connected)
    {
        motor_right();
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Right");
    }
}

void handle_stop()
{
    motor_stop();
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Stop");
}

// void handle_vcc_level()
// {
//   uint16_t vcc_level = ESP.getVcc();
//   std::stringstream ss;
//   ss << vcc_level;                      // Convert the value to a string stream
//   std::string vcc_level_str = ss.str(); // Get the resulting string

//   server.sendHeader("Access-Control-Allow-Origin", "*");
//   server.send(200, "text/plain", vcc_level_str.c_str());
// }

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

void serverSetup()
{
    setupMotores();
    server.on("/", HTTP_GET, handle_connect);
    server.on("/forward", HTTP_POST, handle_forward);
    server.on("/backward", HTTP_POST, handle_backward);
    server.on("/left", HTTP_POST, handle_left);
    server.on("/right", HTTP_POST, handle_right);
    server.on("/stop", HTTP_POST, handle_stop);
    // server.on("/vcc_level", HTTP_POST, handle_vcc_level);
    server.onNotFound(handle_NotFound);
    server.begin();
}
