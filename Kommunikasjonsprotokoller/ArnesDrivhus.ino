/*
Her er kommandoene du kan bruke i programmet, skriv de i serial monitor
M_ON: Sett vifte ON
MOFF: Sett vifte OFF
MAUT: Sett vifte AUTO
SECL: Sett vindu CLOSED
SEOP: Sett vindu OPEN
SEHA: Sett vindu Half-Open
SEAT: Sett vindu AUTO
OLTE: Sett OLED temperatur
OLLI: Sett OLED light
OLWI: Sett OLED vindu
*/


//Oppsett av OLED skjerm
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Definisjoner relatert til skjerm
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
//Oppstart av display, koblest til SDA = A4, SCL = A5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
enum OLEDStates{
  TEMP, 
  LIGHT, 
  FAN,
  WIND
};
enum OLEDStates OLEDState;

//Oppsett av Servo
#include <Servo.h>
Servo myservo;
int servoPin = 9;
enum servoStates{
  CLOSED,
  DEG_45,
  OPEN,
  AUTOS
};
enum servoStates servoState = AUTOS;

//Oppsett av andre pinner
int fotoPin = A2;
int tempPin = A3;
int motorPin = 8;
enum motorStates{
  ON, 
  OFF, 
  AUTOM
};
enum motorStates motorState = AUTOM;

//Timer brukt til å kontrollere intervallet mellom skjerm,motor,servo oppdateringer
long timer;
long intervall = 1000;

void setup() {
  Serial.begin(9600);
  //Setup skjerm
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Standard addresse 0x3C for 128x64
    for(;;);
  }
  //Setter nye skjerminnstillinger
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();

  //Setup Servo
  myservo.attach(servoPin);

  //Annet setup
  pinMode(fotoPin, INPUT);
  pinMode(tempPin, INPUT);
  pinMode(motorPin, OUTPUT);
}

void loop() {
  //Dersom det er seriell informasjon å lese, kall meny funksjonen
  if(Serial.available()>0){
    MENU();
  }
  //Dersom timeren er gått ut kan vi oppdatete skjerm,motor og servo
  if((millis()-timer) >intervall){
    skjermKontroll(); 
    servoKontroll();
    motorKontroll();
    timer = millis();
  }
}

//Håndterer lesing og tolking av seriell data
void MENU(){
  //Leser av innkommende data til karakteren newline er funnet
  String incomingString = Serial.readStringUntil('\n');
  //Kutt av eventuell endedata som ekstra \n eller \r
  incomingString.trim();
  //Konverter til uppercase for å unngå case-sensitivitet
  incomingString.toUpperCase();
  Serial.print("I received: ");
  Serial.println(incomingString);

  //Sammenlign med kommandoord og endre enums etter hvilket som er kommet
  //NB! denne delen av koden kan effektiviserest, men dette ble valgt for å holde den
  //lesbar, i ett større prosjekt ville dette kanskje ikke fungert
  if(incomingString == "M_ON"){motorState = ON;}
  else if(incomingString == "MOFF"){motorState = OFF;}
  else if(incomingString == "MAUT"){motorState = AUTOM;}
  else if(incomingString == "SECL"){servoState = CLOSED;}
  else if(incomingString == "SEOP"){servoState = OPEN;}
  else if(incomingString == "SEHA"){servoState = DEG_45;}
  else if(incomingString == "SEAT"){servoState = AUTOS;}
  else if(incomingString == "OLTE"){OLEDState = TEMP;}
  else if(incomingString == "OLLI"){OLEDState = LIGHT;}
  else if(incomingString == "OLWI"){OLEDState = WIND;}
  else{Serial.println("Kunne ikke tolke forrige melding, prov paa nytt: ");}

}
//Håndterer oppdatering av skjermen
void skjermKontroll(){
  //Vi bruker enumen OLEDState for å bestemme hva som skal visest på skjermen
  //Ellers er casene svært like
  switch(OLEDState){
    case TEMP:
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Temperaturen er: ");
      display.print(temperatur());
      display.print(" grader Celsius");
      display.display();
      break;
    case LIGHT:
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Lysprosenten er: ");
      display.print(fotoProsent());
      display.print(" %");
      display.display();
      break;
    case WIND:
      display.clearDisplay();
      display.setCursor(0, 10);
      display.println("Vinduet er opnet: ");
      display.print(myservo.read());
      display.print(" grader");
      display.display();
      break;
  }
}
//Håndterer oppdatering av servomotor
void servoKontroll(){
  //Vi bruker enumen servoState til å bestemme hvilken vinkel servoen skal ha
  switch(servoState){
    case CLOSED:
      myservo.write(0);
      delay(500);
      break;
    case OPEN:
      myservo.write(90);
      delay(500);
      break;
    case DEG_45:
      myservo.write(45);
      delay(500);
      break;
    case AUTOS:
      //Her bruker vi temperatur for å bestemme vinkelen 
      int temp = temperatur();
      temp = map(temp, 0, 50, 0, 90);
      myservo.write(temp);
      delay(500);
      break;
  }
}
//Håndterer oppdatering av vifte/motor
void motorKontroll(){
  //Vi bruker enumen motorState for å bestemme om viften er av eller på
  switch(motorState){
    case ON:
      vifte(true);
      break;
    case OFF:
      vifte(false);
      break;
    case AUTOM:
      int temp = temperatur();
      //Dersom temperaturen er over 25 OG vinduet IKKE er lukket vil viften skru seg på
      if((temp > 25) && (servoState != CLOSED)){
        vifte(true);
      }
      else{
        vifte(false);
      }
      break;
  }
}
//Leser av fotoresistoren og konverterer dette til en 0-100 verdi
int fotoProsent(){
  int reading = analogRead(fotoPin);
  reading = map(reading, 900, 400, 0, 100);
  return reading;
}
//Leser av TMP36 og returnerer temperatur i celsius
int temperatur(){
  int reading = analogRead(tempPin);
  //Utregning fra datablad
  float spenning = reading * (5.0/1024.0);
  float temperaturC = (spenning- 0.5) *100;
  return temperaturC;
}
//Skrur av og på vifte(er mest her for å unngå rot)
void vifte(bool state){
  digitalWrite(motorPin, state);
}
