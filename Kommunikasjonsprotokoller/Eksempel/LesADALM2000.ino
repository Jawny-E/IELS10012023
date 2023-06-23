void setup(){
  //Start serial, med 8-databit, ingen paritetsbit og 1-stoppbit
	Serial.begin(9600, SERIAL_8N1);
}

void loop(){
  //Dersom det er noe i bufferen
	if(Serial.available()>0){
    //Les serial fram til '!' er mottatt
    String message = Serial.readStringUntil('!');
    //Print det som ble avlest
    Serial.print("Jeg mottok: ");
    Serial.println(message);
  }
  //Skaper timingproblem og kan fjernest, men dette fører lett til SPAM
  delay(500);
}
