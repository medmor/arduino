#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <car.h>

Car car = Car(D7, D1, D2, D5, D3, D4);

boolean is_connected = false;
unsigned long lastHeartbeat = 0;
const unsigned long heartbeatTimeout = 1000;

ESP8266WebServer server(80);

void checkClientConnection()
{
    unsigned long currentMillis = millis();
    if (currentMillis - lastHeartbeat > heartbeatTimeout)
    {
        // Serial.println("Client disconnected");
        is_connected = false;
    }
    else
    {
        // Serial.println("Client connected");
        is_connected = true;
    }
    if (WiFi.status() != WL_CONNECTED || !is_connected)
    {
        car.stop();
    }
}

void serverLoop()
{
    server.handleClient();

    checkClientConnection();

    car.accelerate();
}
void handle_connect()
{
    // server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Connected");
    lastHeartbeat = millis();
}

void handle_forward()
{
    if (is_connected)
    {
        car.forward();
        // server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Forward");
    }
}

void handle_backward()
{
    if (is_connected)
    {
        car.backward();
        // server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Backward");
    }
}

void handle_left()
{
    if (is_connected)
    {
        car.left();
        // server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Left");
    }
}

void handle_right()
{
    if (is_connected)
    {
        car.right();
        // server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", "Right");
    }
}

void handle_stop()
{
    car.stop();
    // server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "Stop");
}

void handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}

void serverSetup()
{
    server.on("/", HTTP_GET, handle_connect);
    server.on("/forward", HTTP_POST, handle_forward);
    server.on("/backward", HTTP_POST, handle_backward);
    server.on("/left", HTTP_POST, handle_left);
    server.on("/right", HTTP_POST, handle_right);
    server.on("/stop", HTTP_POST, handle_stop);
    server.onNotFound(handle_NotFound);
    server.begin();
}
