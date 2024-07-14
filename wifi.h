#include <ESP8266WiFi.h>

const char *ssid = "ZTE_2.4G_YqmMRN";
const char *password = "paEuECPP";

void begin_wifi(){
      WiFi.begin(ssid, password);
}

String get_ip (){
      return WiFi.localIP().toString();
}