#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
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
#pragma endregion

#pragma region Motors ********************

// Ticker accelerator;

// For now my motor driver is malfonctioning. only output A is working.
// So I'm commenting out the other pins.
// Support only forward and backward movements.
// I will fix it later.

// int M1_ENA = D7;
int M1_IN1 = D1;
int M1_IN2 = D2;
// int M2_ENA = D5;
int M2_IN1 = D3;
int M2_IN2 = D4;

// int MIN_SPEED = 200;
// int MAX_SPEED = 255;
// int speed = MIN_SPEED;

void setupMotores()
{
  // pinMode(M1_ENA, OUTPUT);
  // pinMode(M2_ENA, OUTPUT);
  pinMode(M1_IN1, OUTPUT);
  pinMode(M1_IN2, OUTPUT);
  pinMode(M2_IN1, OUTPUT);
  pinMode(M2_IN2, OUTPUT);
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);

  // analogWrite(M1_ENA, speed);
  //    analogWrite(M2_ENA, 255);
}

// void accelerate()
// {
//     log("Accelerate");
//     if (speed < MAX_SPEED)
//     {
//         speed = speed + 10;
//         analogWrite(M1_ENA, speed);
//         // analogWrite(M2_ENA, 255);
//     }
// }

void motor_forward()
{
  log("Forward");

  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  // accelerator.attach(100, accelerate);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
}

void motor_backward()
{
  log("Backward");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  // accelerator.attach(100, accelerate);
  digitalWrite(M2_IN1, HIGH);
  digitalWrite(M2_IN2, LOW);
}

void motor_left()
{
  log("Left");

  digitalWrite(M1_IN1, HIGH);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, HIGH);
}

void motor_right()
{
  log("Right");

  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, HIGH);
  digitalWrite(M2_IN1, HIGH);
  digitalWrite(M2_IN2, LOW);
}

void motor_stop()
{
  log("Stop");
  digitalWrite(M1_IN1, LOW);
  digitalWrite(M1_IN2, LOW);
  digitalWrite(M2_IN1, LOW);
  digitalWrite(M2_IN2, LOW);
  // accelerator.detach();
  // speed = MIN_SPEED;
  // analogWrite(M1_ENA, speed);
}

#pragma endregion

#pragma region Server ********************

const char webpage[] PROGMEM = R"=====(
<html>
	<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Car Controller</title>
	</head>
<body>
	<h1 style="text-align:center;foont-size: 3rem;">سيارة هبة وحمزة العجيبة</h1>
	<main style="display: flex;flex-direction: column;height: 90vh;max-width: 300px;margin: auto;">
      <button
      onmousedown="postDirection('forward')" 
      onmouseup="postDirection('stop')" 
      ontouchstart="postDirection('forward')"
      ontouchend="postDirection('stop')" 
      style="font-size: 5rem;user-select: none;"
    >&uarr;</button>
    <button
      onmousedown="postDirection('backward')" 
      onmouseup="postDirection('stop')" 
      ontouchstart="postDirection('backward')"
      ontouchend="postDirection('stop')" 
      style="font-size: 5rem;user-select: none;"
    >&darr;</button>
    <button 
      onmousedown="postDirection('left')" 
      onmouseup="postDirection('stop')" 
      ontouchstart="postDirection('left')" 
      ontouchend="postDirection('stop')" 
      style="font-size: 5rem;user-select: none;"
    >&larr;</button>
    <button
      onmousedown="postDirection('right')" 
      onmouseup="postDirection('stop')" 
      ontouchstart="postDirection('right')"
      ontouchend="postDirection('stop')" 
      style="font-size: 5rem;user-select: none;"
    >&rarr;</button>
	</main>
  <script>
      function postDirection(url) {
          fetch(window.location.href + url, {method: 'POST'})
          .then(function(response) {
              console.log(response);
          })
          .catch(function(error) {
              console.log(error);
          });
      }
  </script>
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

  begin_wifi();

  setupMotores();

  serverSetup();
}

void loop()
{
  serverLoop();
}

#pragma endregion