#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "./env.h"

IPAddress local_IP(192, 168, 4, 22);

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
    Serial.println("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println(".");
    }
    Serial.println("Connected! IP address: ");
    Serial.println(get_ip());
}

void begin_access_point()
{
    Serial.print("Setting soft-AP configuration ... ");
    Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");

    Serial.print("Setting soft-AP ... ");
    Serial.println(WiFi.softAP(ssid_access_point, password_access_point) ? "Ready" : "Failed!");

    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
}