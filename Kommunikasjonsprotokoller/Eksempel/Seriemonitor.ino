void setup() {
  //Starter Seriekommunikasjon med baudrate 9600
  Serial.begin(9600);
}
void loop() {
  //Dersom det er noe i seriekommunikasjonsbufferen
  if(Serial.available()>0){
  //Les av informasjonen som String
    String melding = Serial.readString();
  //Denne funksjonen fjerner eventuelle '\n' og '\r' fra endene til 
    meldingen
    melding.trim();
  //Print meldingen
    Serial.println("Jeg mottok: " + melding);
  }
}
