#include <ESP8266WiFi.h>

const char* ssid = "Arduino_AP";
const char* password = "12345678";

void setup() {
  Serial.begin(115200);
  Serial.println("Hello");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnecté !");
  Serial.print("IP : ");
  Serial.println(WiFi.localIP());


}

void loop() {

}