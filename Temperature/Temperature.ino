#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "Arduino_AP";
const char* password = "12345678";
WiFiUDP udp;
char packet[255];
char* id = "TJ";

unsigned long deltaTime;
bool sended = false;

int temp = 20;

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

  envoi(strcat("Hello ", id));
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
    Serial.print(deltaTime);Serial.print(" -> ");Serial.println(deltaTime+millis());
  }
  delay(5);
 // ESP.deepSleep(10e6);
}

void loop() {
  int len = udp.parsePacket();
  if (len) {
    udp.read(packet, 255);
    packet[len] = 0;
    Serial.print("P: ");Serial.println(packet);
    //if (strcmp(packet, "led1 on")==0){digitalWrite(LED_BUILTIN, LOW);}
    if (strncmp(packet, "TIME: ", 6)==0){unsigned long heure = strtoul(packet + 6, NULL, 10);deltaTime = heure - millis();}
  }
  if (0 < deltaTime+millis() % 20000 < 2000){
    if (!sended){
      char t[4];
      ultoa(temp, t, 10);
      envoi(strcat(id, t));
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
