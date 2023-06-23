#include <SoftwareSerial.h>
//Start softwareserial på pinne 2-RX og 3-TX
SoftwareSerial mySerial(2, 3);

void setup() {
    //Start serial og softwareserial
    Serial.begin(9600);
    mySerial.begin(9600);
}

void loop() {
//Dersom du kan lese noe fra myserial
if (mySerial.available()) {
      //Les av mySerial og print det til Serial(monitor)
  		Serial.println(mySerial.readString());
  	}
}
