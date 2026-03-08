// resultat du test: 
//  1. Relier D0 et RST   (/!/ deconnecter pendant le flash)
//  2. Inserer resistance 10k ou 47k entre 3V et S0, sinon la carte bug au reveil. Voir lien github joint github.com/esp8266/Arduino/issues/6007
//  3. Pour voir le boot log sur le moniteur, choisir Serial.begin(74880);


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(74880); //115200
}

void loop() {
  //DEBUT test deep sleep
  delay(3000); // 3s
  Serial.println("Start");
  Serial.println(ESP.getResetReason());
  Serial.println(LED_BUILTIN);
  Serial.println("Reveil");
  digitalWrite(LED_BUILTIN, LOW);
  delay(3000); // 3s
  Serial.println("Sleep");
  ESP.deepSleep(5e6); // 1s
  Serial.println("Reveil");
  blink();
  blink();
  blink();
  // FIN

}

void blink(){
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
}
