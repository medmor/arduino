#include <ESP8266WiFi.h>

const char *ssid = "ZTE_2.4G_YqmMRN";
const char *password = "paEuECPP";

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
