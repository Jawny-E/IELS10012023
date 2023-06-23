#include <Wire.h>
int x = 0;

void setup() {
  //Start I2C-buss
  Wire.begin(); 
}
void loop() {
  Wire.beginTransmission(9);  // Start dataoverføring med adresse 9
  Wire.write(x);              // Send x til adresse 9
  Wire.endTransmission();    // Stopp overføring
  x++; //Endre x
  if (x > 5) x = 0; // Resetter x om den blir høgere enn 5
  delay(1);
}
