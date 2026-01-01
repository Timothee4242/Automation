#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


const char* ssid = "Arduino_AP";
const char* password = "12345678";

WiFiUDP udp;

char packet[255];

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
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

  udp.begin(4210);
  delay(500);
}

void loop() {
  int len = udp.parsePacket();
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    Serial.println(packet);
    if (strcmp(packet, "led1 on")==0){digitalWrite(LED_BUILTIN, LOW);}
    if (strcmp(packet, "led1 off")==0){digitalWrite(LED_BUILTIN, HIGH);}
  }
  delay(5);time(nullptr);
}
