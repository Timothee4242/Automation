#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <user_interface.h>


WiFiUDP udp;


const char* ssid = "Arduino_AP";
const char* password = "12345678";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.print("IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());

  udp.begin(4210);
}

void loop() {
  envoi("led1 on");
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  
  envoi("led1 off");
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void envoi(const char* message){
  udp.beginPacket("192.168.4.255", 4210); //IP Broadcast adress - to send packet to everyone
  udp.print(message);
  udp.endPacket();
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
