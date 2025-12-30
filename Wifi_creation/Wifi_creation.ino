#include <ESP8266WiFi.h>

const char* ssid = "Arduino_AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.print("IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());
}

void loop() {

}