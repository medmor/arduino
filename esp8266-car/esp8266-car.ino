#include <sstream>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include "./env.h"

#pragma region Wifi ********************

// IPAddress local_IP(192, 168, 4, 22);
// IPAddress gateway(192, 168, 4, 9);
// IPAddress subnet(255, 255, 255, 0);

IPAddress staticIP(192, 168, 1, 2);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

String get_ip()
{
  return WiFi.localIP().toString();
}

void begin_wifi()
{
  WiFi.disconnect();
  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.begin(ssid, password);
  // // // Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    // // // Serial.println(".");
  }
  // // // Serial.println("Connected! IP address: ");
  // // // Serial.println(get_ip());
}

// void begin_access_point()
// {
//   // // // Serial.print("Setting soft-AP configuration ... ");
//   // // // Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

//   // // // Serial.print("Setting soft-AP ... ");
//   // // // Serial.println(WiFi.softAP(ssid_access_point, password_access_point) ? "Ready" : "Failed!");
//   // WiFi.softAP(ssid);
//   // WiFi.softAP(ssid, password, channel, hidden, max_connection)

//   // // // Serial.print("Soft-AP IP address = ");
//   // // // Serial.println(WiFi.softAPIP());
// }
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
  // // // Serial.println("Accelerate");
  if (speed < MAX_SPEED)
  {
    speed = speed + ACCELERATION;
    analogWrite(M1_ENA, speed);
    analogWrite(M2_ENA, speed);
  }
}

void motor_forward()
{
  // // Serial.println("Forward");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_backward()
{
  // // Serial.println("Backward");

  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M2_IN1, HIGH);
  digitalWrite(M2_IN2, LOW);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_left()
{
  // // Serial.println("Left");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_right()
{
  // // Serial.println("Right");

  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
  accelerator.attach(TIME_TO_ACCELERATE, accelerate);
}

void motor_stop()
{
  // // Serial.println("Stop");
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

#pragma endregion

#pragma region Setup ********************

void setup()
{
  Serial.begin(115200);

  begin_wifi();

  // begin_access_point();

  setupMotores();

  serverSetup();
}

void loop()
{
  serverLoop();
  checkClientConnection();
  if (WiFi.status() != WL_CONNECTED || !is_connected)
  {
    motor_stop();
  }
}

#pragma endregion