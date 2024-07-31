#include <car_server.h>
#include <car.h>
#include <LittleFS.h>

void CarServer::setup()
{
    LittleFS.begin();
    server.on("/", HTTP_GET, [this]()
              { handle_index(); });
    server.on("/forward", HTTP_GET, [this]()
              { handle_forward(); });
    server.on("/backward", HTTP_GET, [this]()
              { handle_backward(); });
    server.on("/left", HTTP_GET, [this]()
              { handle_left(); });
    server.on("/right", HTTP_GET, [this]()
              { handle_right(); });
    server.on("/stop", HTTP_GET, [this]()
              { handle_stop(); });
    server.onNotFound([this]()
                      { handle_NotFound(); });
    server.begin();
}

void CarServer::loop()
{
    server.handleClient();
}

void CarServer::handle_index()
{
    if (!LittleFS.begin())
    {
        Serial.println("Failed to mount file system");
        return;
    }

    File file = LittleFS.open("/index.html", "r");
    if (file)
    {
        server.streamFile(file, "text/html");
        file.close();
    }
    else
    {
        Serial.println("Failed to open file");
    }
}

void CarServer::handle_forward()
{
    if (is_connected)
    {
        car.forward();
        server.send(200, "text/plain", "Forward");
    }
}

void CarServer::handle_backward()
{
    if (is_connected)
    {
        car.backward();
        server.send(200, "text/plain", "Backward");
    }
}

void CarServer::handle_left()
{
    if (is_connected)
    {
        car.left();
        server.send(200, "text/plain", "Left");
    }
}

void CarServer::handle_right()
{
    if (is_connected)
    {
        car.right();
        server.send(200, "text/plain", "Right");
    }
}

void CarServer::handle_stop()
{
    car.stop();
    server.send(200, "text/plain", "Stop");
}

void CarServer::handle_NotFound()
{
    server.send(404, "text/plain", "Not found");
}
