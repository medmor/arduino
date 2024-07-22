#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "./env.h"

#pragma region Log ********************
void logSetup()
{
  Serial.begin(115200);
}

void log(String message)
{
  Serial.println(message);
}
#pragma endregion

#pragma region Wifi ********************

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 9);
IPAddress subnet(255, 255, 255, 0);

String get_ip()
{
  return WiFi.localIP().toString();
}

void begin_wifi()
{
  WiFi.begin(ssid, password);
  log("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    log(".");
  }
  log("Connected! IP address: ");
  log(get_ip());
}

void begin_access_point()
{
  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ssid_access_point, password_access_point) ? "Ready" : "Failed!");
  // WiFi.softAP(ssid);
  // WiFi.softAP(ssid, password, channel, hidden, max_connection)

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());
}
#pragma endregion

#pragma region Motors ********************

Ticker accelerator;

int M1_ENA = D7;
int M1_IN1 = D1;
int M1_IN2 = D2;

int M2_ENA = D5;
int M2_IN1 = D3;
int M2_IN2 = D4;

int MIN_SPEED = 150;
int MAX_SPEED = 255;
int ACCELERATION = 5;
float TIME_TO_ACCELERATE = .1;
int speed = MIN_SPEED;

void setupMotores()
{
  pinMode(M1_ENA, OUTPUT);
  pinMode(M2_ENA, OUTPUT);
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);

  analogWrite(M1_ENA, speed);
  analogWrite(M2_ENA, speed);
}

void accelerate()
{
  log("Accelerate");
  if (speed < MAX_SPEED)
  {
    speed = speed + ACCELERATION;
    analogWrite(M1_ENA, speed);
    analogWrite(M2_ENA, speed);
  }
}

void motor_forward()
{
  log("Forward");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_backward()
{
  log("Backward");

  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M2_IN1, HIGH);
  digitalWrite(M2_IN2, LOW);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_left()
{
  log("Left");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_right()
{
  log("Right");

  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_stop()
{
  log("Stop");
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);
  accelerator.detach();
  speed = MIN_SPEED;
  analogWrite(M1_ENA, speed);
  analogWrite(M2_ENA, speed);
}

#pragma endregion

#pragma region Server ********************

const char webpage[] PROGMEM = R"=====(
<!-- A simple controller for the car. Transformed to android app using webintoapp website. -->
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>السيارة العجيبة</title>
</head>
<body>
<h1>عليك تنزيل تطبيق سيارة هبة وحمزة العجيبة للتحكم في السيارة</h1>
</body>
</html>
)=====";

ESP8266WebServer server(80);
void handle_connect()
{
  server.send(200, "text/html", webpage);
}

void serverLoop()
{
  server.handleClient();
}

void handle_forward()
{
  motor_forward();
  server.send(200, "text/plain", "Forward");
}

void handle_backward()
{
  motor_backward();
  server.send(200, "text/plain", "Backward");
}

void handle_left()
{
  motor_left();
  server.send(200, "text/plain", "Left");
}

void handle_right()
{
  motor_right();
  server.send(200, "text/plain", "Right");
}

void handle_stop()
{
  motor_stop();
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

#pragma endregion

#pragma region Setup ********************

void setup()
{
  logSetup();

  // begin_wifi();
  begin_access_point();

  setupMotores();

  serverSetup();
}

void loop()
{
  serverLoop();
}

#pragma endregion