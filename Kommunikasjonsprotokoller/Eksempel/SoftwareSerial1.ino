#include <SoftwareSerial.h>
//Start softwareserial på pinne 2-RX og 3-TX
SoftwareSerial mySerial(2,3);
void setup(){
	//Start seriekommunikasjon med seriemonitor
  Serial.begin(9600);
  //Start seriekommunikasjon med pinne 2 og 3
  mySerial.begin(9600);
}

void loop(){
  //Print til pinne 3
	mySerial.println("Hei, dette er sender!");
	delay(1000);
}
