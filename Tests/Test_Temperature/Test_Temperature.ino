#include <ESP8266WiFi.h>

//Temperature
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
float temp;
float hum;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); 
  dht.begin(); //initie le capteur de temperature
  Serial.begin(115200);
  Serial.println("Hello");
}

void loop() {
  temp=dht.readTemperature();
  hum=dht.readHumidity();
  Serial.println("Temperature = " + String(temp)+" °C");
  Serial.println("Humidite = " + String(hum)+" %");
  delay(1000);
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
}
