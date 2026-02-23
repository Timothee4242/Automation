#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

#define EEPROM_SIZE 96
#define DAILY_SLEEP 86400e6
#define RETRY_SLEEP 3600e6

const char* serverName = "http://10.0.4.45:5000/data";

struct WifiData {
  char ssid[32];
  char pass[64];
};

WifiData wifiData;

void setup() {

  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, wifiData);
  Serial.begin(115200);
  WiFi.begin(wifiData.ssid, wifiData.pass);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    Serial.print("Wifi non connecte, try ");
    Serial.println(timeout);
    delay(500);
    timeout++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("# Wifi non connecte, nouvel essai dans une heure#");
    ESP.deepSleep(RETRY_SLEEP);
  }
  Serial.println(WiFi.localIP());
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");
  String json = "[12,45,78,90]";
  int httpCode = http.POST(json);
  http.end();
  Serial.print("httpCode: ");
  Serial.print(httpCode);
  if (httpCode > 0) {
    Serial.println("  Je retourne dormir pour la journee");
    ESP.deepSleep(DAILY_SLEEP);
  } else {
    Serial.println("  Je reesaie dans une heure");
    ESP.deepSleep(RETRY_SLEEP);
  }
}

void loop() {}
