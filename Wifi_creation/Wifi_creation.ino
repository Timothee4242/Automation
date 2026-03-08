#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <user_interface.h>
bool verbos=true;

WiFiUDP udp;

const char* ssid = "Arduino_AP";
const char* password = "12345678";
char packet[255];

///////// WiFi maison ////////////
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
#define MAX_LOG 100 
String logLines[MAX_LOG]; // tableau pour stocker les lignes
int nbLog = 0;            // compteur de lignes écrites

/////////Gestion des informations////////////
float t_sbb[1440]; //1440: minutes in one day
float h_sbb[1440];
int temps_fin=0;
int temps=0;
char* statut="-RELAI_LOW";

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  WiFi.softAP(ssid, password);

  Serial.print("IP du point d'accès : ");
  Serial.println(WiFi.softAPIP());

  udp.begin(4210);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("HelloWorld");

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
    char msg[255];
    strcpy(msg, "TIME: ");
    strcat(msg, t);
    envoi(msg);
 // envoi(strcat("TIME: ",t));
}

void loop() {
  int len = udp.parsePacket();
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    Serial.print("P: ");Serial.println(packet);
    if (strncmp(packet, "TJ", 2)==0){float temp = atof(packet + 2);}
    if (strncmp(packet+3, "tmp", 3)==0){
      float temp = atof(packet + 13);
      //Serial.print("temperature: ");Serial.println(temp);
      if (strncmp(packet+6, "SBB", 3)==0){
        t_sbb[0] = temp;
        Serial.print("temperature sbb: ");Serial.println(temp);
      }
    }
    if (strncmp(packet+3, "hum", 3)==0){
      float hum = atof(packet + 13);
      //Serial.print("temperature: ");Serial.println(temp);
      if (strncmp(packet+6, "SBB", 3)==0){
        h_sbb[0] = hum;
        Serial.print("humidité sbb: ");Serial.println(hum);
      }
    }
    temps=int(millis()/1000);Serial.print("Temps:");Serial.println(temps);
    if (h_sbb[0]>60){
      temps_fin=temps+10; //
      if(verbos){Serial.println("H>60");}
    }
    if (temps_fin>temps and strncmp(statut, "-RELAI_HIGH", 10)!=0){
      envoi("HUB:VMC-RELAI_HIGH");
      strcpy(statut,"-RELAI_HIGH");
    }
    if (temps_fin<=temps and strncmp(statut, "-RELAI_LOW", 9)!=0){
      envoi("HUB:VMC-RELAI_LOW");   
      strcpy(statut,"-RELAI_LOW");  
    }
    if (strncmp(packet, "Hello", 5)==0){
      Serial.println("reçu");
      delay(500);
      char t[12];
      ultoa(millis(),t,10);
        char msg[255];
        strcpy(msg, "TIME: ");
        strcat(msg, t);
        envoi(msg);
      //envoi(strcat("TIME: ",t));
      }
  }
  //Serial.print("Time: ");Serial.println(millis());

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
  if (verbos){
  Serial.print("envoi:");
  Serial.println(message);
  String msg=String(message); 
  addLog(msg);
  }
}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  digitalWrite(LED_BUILTIN, HIGH);
}

void envoi_synthese(){
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, wifiData);
  WiFi.begin(wifiData.ssid, wifiData.pass);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    Serial.print("Wifi maison non connecte, try ");
    Serial.println(timeout);
    delay(500);
    timeout++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("# Wifi maison non connecte, nouvel essai dans une heure#");
    ESP.deepSleep(RETRY_SLEEP);
  }
  Serial.println(WiFi.localIP());
  Serial.println("### logLines ###");
  Serial.println(logLines);
  Serial.println("################");
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  http.addHeader("Content-Type", "application/json");
  //String json = "[12,45,78,90]";
  int httpCode = http.POST(logLines);
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
void addLog(const String &message) {
  if (nbLog < MAX_LOG) {
    logLines[nbLog++] = message;  // ajouter au tableau
  } else {
    // si plein, décaler tout et ajouter à la fin
    for (int i = 1; i < MAX_LOG; i++) {
      logLines[i - 1] = logLines[i];
    }
    logLines[MAX_LOG - 1] = message;
  }
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
