#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "DHT.h"

#define DHTPIN D2
#define DHTTYPE DHT11
const char* ssid = "Arduino_AP";
const char* password = "12345678";

WiFiUDP udp;

char packet[255];
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello");
  dht.begin();
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

  envoiHub("Hello");
 // ESP.deepSleep(10e6);
  delay(10000);
}

void loop() {
  int len = udp.parsePacket();
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    //Serial.print("recoit:");
    Serial.println(packet);
    if (strcmp(packet, "led1 on")==0){digitalWrite(LED_BUILTIN, LOW);}
    if (strcmp(packet, "led1 off")==0){digitalWrite(LED_BUILTIN, HIGH);}
    if (strcmp(packet, "Blink")==0){blink();}
  }
  Serial.println("Temperature = " + String(dht.readTemperature())+" °C");
  Serial.println("Humidite = " + String(dht.readHumidity())+" %");
  delay(5);
}

void envoiHub(const char* message){
  udp.beginPacket("192.168.4.1", 4210); //IP Broadcast adress - to send packet to everyone
  udp.print(message);
  //Serial.print("envoie:");
  Serial.println(message);
  udp.endPacket();
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);
}
