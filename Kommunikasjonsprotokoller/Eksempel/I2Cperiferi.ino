#include <Wire.h>
int x = 0;

void setup() {
  //Kobl på I2C-bussen med adresse 9
  Wire.begin(9); 
  //Funksjonen mottarmelding vil kjøre når det mottas en melding 
  Wire.onReceive(mottarMelding);
}
void mottarMelding(int bytes) {
  x = Wire.read();    // Les av en karakter fra I2C-bussen
}
void loop() {
  ; //Gjør ingenting(utenom når melding kommer)
}
