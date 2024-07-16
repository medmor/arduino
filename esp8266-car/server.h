#include <ESP8266WebServer.h>

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