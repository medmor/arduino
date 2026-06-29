#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <LittleFS.h>
#include "car.h"

// ---- WiFi AP config ----
const char *ssid_access_point = "ESP8266-Car";
const char *password_access_point = "12345678";

IPAddress local_IP(192, 168, 4, 22);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

// ---- Car ----
// NodeMCU ESP8266 D1,D2,D3,D4 map to GPIO5,GPIO4,GPIO0,GPIO2.
Car car(D1, D2, D3, D4);

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

const char INDEX_HTML[] PROGMEM =
    "<!DOCTYPE html><html><head><meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
    "<title>ESP8266 Car</title><style>"
    "body{margin:0;padding:0;background:#101418;color:#fff;font-family:sans-serif}"
    ".wrap{display:flex;flex-direction:column;align-items:center;padding-top:20px}"
    "h2{margin:10px 0 4px}"
    "#st{color:#9fd3ff;font-size:14px;margin-bottom:20px}"
    ".pad{width:270px;height:270px;position:relative}"
    ".btn{position:absolute;width:90px;height:90px;border-radius:45px"
    ";background:#1f6feb;color:#fff;font-size:34px;font-weight:700"
    ";display:flex;align-items:center;justify-content:center"
    ";border:none;user-select:none;touch-action:none}"
    ".up{top:0;left:90px}.down{bottom:0;left:90px}"
    ".left{left:0;top:90px}.right{right:0;top:90px}"
    ".stop{left:90px;top:90px;background:#444b53}"
    "#dot{display:block;margin:16px auto 0;width:40px;height:40px;border-radius:20px"
    ";background:#444}"
    "</style></head><body><div class='wrap'><h2>ESP8266 Car</h2>"
    "<div id='st'>Not connected</div><div class='pad'>"
    "<button class='btn up'    data-cmd='forward'>  &#9650;</button>"
    "<button class='btn down'  data-cmd='backward'> &#9660;</button>"
    "<button class='btn left'  data-cmd='left'>     &#9664;</button>"
    "<button class='btn right' data-cmd='right'>    &#9654;</button>"
    "<button class='btn stop'  data-cmd='stop'>     &#9632;</button>"
    "</div><div id='dot'></div></div>"
    "<script>"
    "let ws,gateway='ws://'+location.hostname+':81',dot=document.getElementById('dot'),st=document.getElementById('st');"
    "function init(){ws=new WebSocket(gateway);ws.onopen=()=>{dot.style.background='#4ade80';st.textContent='Connected'};"
    "ws.onclose=()=>{dot.style.background='#ef4444';st.textContent='Disconnected';setTimeout(init,1000)};}"
    "window.addEventListener('load',init);"
    "document.querySelectorAll('.btn').forEach(b=>{"
    "const c=b.dataset.cmd;"
    "const on=()=>{if(ws&&ws.readyState===1)ws.send(c)};"
    "const off=()=>{if(ws&&ws.readyState===1)ws.send('stop')};"
    "b.addEventListener('mousedown',on);b.addEventListener('mouseup',off);b.addEventListener('mouseleave',off);"
    "b.addEventListener('touchstart',e=>{e.preventDefault();on()},{passive:false});"
    "b.addEventListener('touchend',e=>{e.preventDefault();off()},{passive:false});});"
    "</script></body></html>";

void handleIndex()
{
    server.send_P(200, "text/html", INDEX_HTML);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_TEXT:
    {
        String cmd = String((char *)payload);
        cmd.trim();
        if (cmd == "forward")
            car.forward();
        else if (cmd == "backward")
            car.backward();
        else if (cmd == "left")
            car.left();
        else if (cmd == "right")
            car.right();
        else if (cmd == "stop")
            car.stop();
        break;
    }
    default:
        break;
    }
}

void setup()
{
    Serial.begin(115200);
    delay(200);
    Serial.println("Starting ESP8266 Car (WiFi/WebSocket)...");

    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_IP, gateway, subnet);
    WiFi.softAP(ssid_access_point, password_access_point);

    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, handleIndex);
    server.begin();

    webSocket.begin();
    webSocket.onEvent(webSocketEvent);

    Serial.println("Ready.");
}

void loop()
{
    server.handleClient();
    webSocket.loop();

    if (webSocket.connectedClients() == 0)
    {
        car.stop();
    }
}