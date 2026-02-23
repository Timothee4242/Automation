// Pour configurer le wifi de la maison sur la carte arduino
#include <EEPROM.h>

#define EEPROM_SIZE 96

struct WifiData {
  char ssid[32];
  char pass[64];
};

void setup() {

  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);

  WifiData data;

  Serial.println("Entrer SSID:");
  while (!Serial.available());
  String s = Serial.readStringUntil('\n');
  s.trim();
  s.toCharArray(data.ssid, 32);

  Serial.println("Entrer PASSWORD:");
  while (!Serial.available());
  String p = Serial.readStringUntil('\n');
  p.trim();
  p.toCharArray(data.pass, 64);

  EEPROM.put(0, data);
  EEPROM.commit();

  Serial.println("Sauvegarde OK");
}

void loop() {}
