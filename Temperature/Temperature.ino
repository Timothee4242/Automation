#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

//Temperature
#include "DHT.h"
#define DHTPIN D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Arduino_AP";
const char* password = "12345678";
WiFiUDP udp;
char packet[255];
const char* id = "SBB"; //salle de bain bas
unsigned long deltaTime;
bool sended = false;
float temp;
float hum;

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin(); //initie le capteur de temperature
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

  char msg[20];
  strcpy(msg, "Hello ");
  strcat(msg, id);
  envoi(msg);
  bool ok=false;
  while(!ok){
    int len = udp.parsePacket();
    if (len) {
      udp.read(packet, 255);
      packet[len] = 0;
      Serial.println(packet);
      if (strncmp(packet, "TIME: ", 6)==0){
        unsigned long heure = strtoul(packet + 6, NULL, 10);
        deltaTime = heure - millis();
        ok = true;
      }
    }
    Serial.print(deltaTime);Serial.print(" ; ");Serial.println(deltaTime+millis());
    delay(50);
  }
  delay(5);
 // ESP.deepSleep(10e6);
}

void loop() {
  int len = udp.parsePacket();
  char msg[255];
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    Serial.print("P: ");Serial.println(packet);
    //if (strcmp(packet, "led1 on")==0){digitalWrite(LED_BUILTIN, LOW);}
    if (strncmp(packet, "TIME: ", 6)==0){unsigned long heure = strtoul(packet + 6, NULL, 10);deltaTime = heure - millis();}
  }
  if ((deltaTime+millis())%20000 < 2000){
    if (!sended){
      Serial.println("Sending");
      temp=dht.readTemperature();
      hum=dht.readHumidity();
      Serial.println("Temperature = " + String(temp)+" °C");
      Serial.println("Humidite = " + String(hum)+" %");
      creemsg(msg,"tmpSBB----",temp);
      envoi(msg);
      creemsg(msg,"humSBB----",hum);
      envoi(msg);
    }
    sended = true;
  }
  else{sended = false;}
  delay(5);
}

void envoiHub(const char* message){
  udp.beginPacket("192.168.4.1", 4210); //IP HUB adress - to send packet to the HUB
  udp.print(message);
  udp.endPacket();
}
void creemsg(char *dest, const char* nom,float data){
  char dat[4]; //data
  dtostrf(data, 0, 2, dat); 
  strcpy(dest, id);
  strcat(dest, nom);
  strcat(dest, dat);
}
void envoi(const char* message){
  udp.beginPacket("192.168.4.255", 4210); //IP Broadcast adress - to send packet to everyone
  udp.print(message);
  udp.endPacket();
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
}
