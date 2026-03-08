
//Relai
#define RELAIPIN D1

void setup(){
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(RELAIPIN,OUTPUT);

}

void loop() {
  digitalWrite(RELAIPIN, HIGH);
  delay(500);  
  digitalWrite(RELAIPIN, LOW);
  delay(500);
}
