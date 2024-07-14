#include <ESP8266WebServer.h>
#include "motor.h"

const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>Car Controller</title>
	</head>
<body>
	<main style="display:flex;gap: 10px;flex-direction:column;max-width: 300px;margin: auto;">
		<h1 style="text-align:center;foont-size: 3rem;">سيارة هبة وحمزة العجيبة</h1>
    <button 
      onmousedown="postDirection('forward')" 
      onmouseup="postDirection('stop')" 
      touchstart="postDirection('forward')" 
      touchend="postDirection('stop')" 
      style="height: 50vh;font-size: 10rem;"
    >⮝</button>
    <button
      onmousedown="postDirection('backward')" 
      onmouseup="postDirection('stop')" 
      touchstart="postDirection('backward')"
      touchend="postDirection('stop')" 
      style="height: 50vh; font-size: 10rem;"
    >⮟</button>
	</main>
  <script>
      function postDirection(url) {
          fetch(window.location.href + url, {method: 'POST'})
          .then(function(response) {
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

// void handle_left()
// {
//   motor_left();
//   server.send(200, "text/plain", "Left");
// }

// void handle_right()
// {
//   motor_right();
//   server.send(200, "text/plain", "Right");
// }

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
  // server.on("/left", HTTP_POST, handle_left);
  // server.on("/right", HTTP_POST, handle_right);
  server.on("/stop", HTTP_POST, handle_stop);
  server.onNotFound(handle_NotFound);
  server.begin();
}