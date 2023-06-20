//Setter opp servo
#include <Servo.h>
Servo myservo;
int servoAngle;
//Analoge Inngangspinner 
int potpin = A3;
int fotpin = A4;
//Denne variabelen skal kontrollere switch-case
int kontroll;

void setup() {
  //Servoen koblest til pinne 9
  myservo.attach(9);
  //Start seriell kommunikasjon (Husk å ha Serial Monitor åpen)
  Serial.begin(9600);
}

void loop() {
  //Programmet er plasser i en funksjon for å gjøre videreutvikling enklere
  stateControl();
}

void stateControl(){
  switch(kontroll){
    case 'D':
      //Oppstartsmelding
      Serial.println("Dette er casen for aa endre vinkler");
      Serial.println("Trykk 0: For aa sette 0 grader");
      Serial.println("Trykk 1: For aa sette 90 grader");
      Serial.println("Trykk 2: For aa sette 180 grader");
      Serial.println("Trykk Q: For aa gaa tilbake til hovedmeny");
      //So lenge det ikkje er mottatt Q
      while(kontroll != 'Q'){
        //Les av seriell inngang om det er noko tilgjengleg
        if(Serial.available()>0){
          kontroll = Serial.read();
        }
        //Sjekk om kontroll er 0,1 eller 2, då skal vinkel justerast
        if(kontroll == '0'){
          servoAngle = 0;
        }
        else if(kontroll == '1'){
          servoAngle = 90;
        }
        else if(kontroll == '2'){
          servoAngle = 180;
        }
        //Sett vinkel på motoren
        myservo.write(servoAngle);
      }
      //Kontroll er satt til 0 for å sikre at neste case er 'default'
      kontroll = 0;
      break;
    case 'A':
      //Oppstartsmelding
      Serial.println("Her kan du justere vinkelen med 10-grader");
      Serial.println("Skriv + for aa legge til 10-grader");
      Serial.println("Skriv - for aa trekke fra 10-grader");
      Serial.println("Trykk Q: For aa gaa tilbake til hovedmeny");
      //So lenge det ikkje er mottatt Q
      while(kontroll != 'Q'){
        //Les av seriell inngang om det er noko tilgjengleg
        if(Serial.available()>0){
          kontroll = Serial.read();
        }
        //Sjekk om kontroll tilsvarer ein av dei gitte kommandoane
        //Endre servovinkel om det stemmer
        if(kontroll == '-'){
          servoAngle = servoAngle - 10;
          kontroll = 0;
        }
        else if(kontroll == '+'){
          servoAngle = servoAngle + 10;
          kontroll = 0;
        }
        //Sett vinkel på motoren
        myservo.write(servoAngle);
      }
      //Kontroll er satt til 0 for å sikre at neste case er 'default'
      kontroll = 0;
      break;
    case 'S':
      //Oppstartsmelding
      Serial.println("Dette er casen for aa sweepe mellom 0 og 180 grader");
      Serial.println("Trykk Q: For aa gaa tilbake til hovedmeny");
      //So lenge det ikkje er mottatt Q
      while(kontroll != 'Q'){
        //Les av seriell inngang om det er noko tilgjengleg
        if(Serial.available()>0){
          kontroll = Serial.read();
        }
        //Juster vinkel frå 0 til 180
        for(servoAngle = 0; servoAngle <= 180; servoAngle += 1) {
          myservo.write(servoAngle); 
          delay(5);
        }
        //Juster vinkel frå 180 til 0
        for(servoAngle = 180; servoAngle >= 0; servoAngle -= 1) { 
          myservo.write(servoAngle);  
          delay(5);                      
        }
        //MERK! det kan være litt delay mellom Q er tasta inn og man går tilbake til meny
      }
      //Kontroll er satt til 0 for å sikre at neste case er 'default'
      kontroll = 0;
      break; 
    case 'P':
      //Oppstartsmelding
      Serial.println("Dette er casen for aa justere med potentiometer");
      Serial.println("Trykk Q: For aa gaa tilbake til hovedmeny");
      //So lenge det ikkje er mottatt Q
      while(kontroll != 'Q'){
        //Les av seriell inngang om det er noko tilgjengleg
        if(Serial.available()>0){
          kontroll = Serial.read();
        }
        //Les av potentiometerpinnen
        servoAngle = analogRead(potpin);
        //Bruk map-funksjonen for å omgjere analog verdi til vinkel
        servoAngle = map(servoAngle, 0, 1023, 0, 180);
        //Sett ny servovinkel
        myservo.write(servoAngle);
      }
      //Kontroll er satt til 0 for å sikre at neste case er 'default'
      kontroll = 0;
      break;
    case 'F':
      //Oppstartsmelding
      Serial.println("Dette er casen for aa justere vinkel med fotoresistor");
      Serial.println("Trykk Q: For aa gaa tilbake til hovedmeny");
      //So lenge det ikkje er mottatt Q
      while(kontroll != 'Q'){
        //Les av seriell inngang om det er noko tilgjengleg
        if(Serial.available()>0){
          kontroll = Serial.read();
        }
        //Les av servopinnen
        servoAngle = analogRead(fotpin);
        //Bruk map-funksjonen for å omgjere analog verdi til vinkel
        servoAngle = map(servoAngle, 0, 1023, 0, 180);
        //Sett ny servovinkel
        myservo.write(servoAngle);
      }
      //Kontroll er satt til 0 for å sikre at neste case er 'default'
      kontroll = 0;
      break;
    default:
      //Denne meldingen vil vise ved oppstart
      Serial.println("Venter paa kommando");
      Serial.println("-----Kommandoliste----- ");
      Serial.println("D: Sett servoen til ein fast verdi");
      Serial.println("A: Juster servoen sin vinkel med 10-grader");
      Serial.println("S: Servoen sweeper mellom 0 og 180 grader");
      Serial.println("P: Juster servoen med ett potentiometer");
      Serial.println("F: Juster servoen med en fotoresistor");
      //Gjer ingenting til det er kommt seriell data
      while(Serial.available()==0){
        ;
      }
      //Neste case er fastsett av seriell data
      kontroll = Serial.read();
      break;
  }
}
