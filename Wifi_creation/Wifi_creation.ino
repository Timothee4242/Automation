#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <user_interface.h>


WiFiUDP udp;


const char* ssid = "Arduino_AP";
const char* password = "12345678";
char packet[255];
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.print("IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());

  udp.begin(4210);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("Hello");

    bool ok = false;
  while(!ok){
    int len = udp.parsePacket();
    if (len) {
      udp.read(packet, 255);
      packet[len] = 0;
      Serial.println(packet);
      if (strncmp(packet, "Hello", 5)==0){ok = true;Serial.println("reçu");}
    }
    delay(5);
  }
  
  delay(500);
  char t[12];
  ultoa(millis(),t,10);
  envoi(strcat("TIME: ",t));
}

void loop() {
  int len = udp.parsePacket();
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    Serial.println(packet);
    if (strncmp(packet, "T", 1)==0){
      if (strncmp(packet+1, "J", 1)==0){
        unsigned long temp = strtoul(packet + 2, NULL, 10);
        Serial.print(temp);
      }
    }
  }

  //blink();

 /* envoi("led1 on");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  
  envoi("led1 off");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);*/
  delay(5);
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
/*
IPAddress afficherClients[10]() {
  struct station_info *stat_info = wifi_softap_get_station_info();
  struct station_info *it = stat_info;
  IPAddress ips[15];
  i=0;
  while (it != NULL) {
    IPAddress ip = IPAddress(it->ip.addr);
    Serial.print("Client connecté : ");
    Serial.println(ip);
    ips[i] = ip;
    it = STAILQ_NEXT(it, next);
    i++;
  }
  wifi_softap_free_station_info();
  return ips;
}/**/
