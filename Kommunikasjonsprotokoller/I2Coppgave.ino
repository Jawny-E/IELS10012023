//Bibliotek relatert til skjermen
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//Definisjoner relatert til skjerm
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64 
//Oppstart av display, koblest til SDA = A4, SCL = A5
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Andre pinner brukt i programmet
int fotpin = A3;
int tiltpin = 9;
int knappin = 8;
//Tellere for antall innganger
int tiltAktiv = 0;
int lysAktiv = 0;
//Desse globale variablene kontrollerer skjermen
int cases = 3;    //Det totale tallet på cases
int kontroll = 1; //Tallet beskriver hvilken case man er i NÅ
//Timerere som kontrollerer når skjerm og knapp kan oppdaterest
long timer1 = 0;
long timer2 = 0;
long intervall = 1000;


void setup() {
  Serial.begin(9600);
  //Setter sensorpinnene som input
  pinMode(knappin, INPUT);
  pinMode(fotpin, INPUT);
  pinMode(tiltpin, INPUT);
  //Starter opp skjerm
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Standard addresse 0x3C for 128x64
    Serial.println(F("Klarte ikke å koble til skjermen"));
    for(;;);
  }
  //Setter nye skjerminnstillinger
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.display();
}

void loop() {
  //Sjekker om knappe trykk er skjedd
  sjekKnapp();
  //Oppdaterer skjermen, bruker finnTemperatur og finnLysstyrke direkte for å unngå dobbeltskriving
  skjermKontroll();
  //Sjekk sensorer
  sjekkSensorer();
}

void sjekKnapp(){
  //Dersom knappen er trykkt OG timeren er gått ut -> Oppdater kontroll
  if((digitalRead(knappin)) && ((millis()-timer1) > intervall)){
    //Viss vi er gått opp til cases -> gå tilbake til 1
    if(kontroll == cases){
      kontroll = 1;
    }
    //Ellers neste case er +1 fra forrige
    else {
      kontroll +=  1;
    }
    //Oppdater timer
    timer1 = millis();
  }
}

void skjermKontroll(){
    //Her visest en liten oppstartsmelding på skjermen
    if(kontroll == 1){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("IDLE");
      display.display();
    }
    //Dersom kontroll er 2 vil tiltinformasjon visest på skjermen
    else if(kontroll == 2){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Tiltsensor aktivert: ");
      display.print(tiltAktiv);
      display.print(" ganger");
      display.display();
    }
    //Dersom kontroll er 3 vil fotoresistorinformasjon visest på skjermen
    else if(kontroll == 3){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Fotoresistor aktivert: ");
      display.print(lysAktiv);
      display.print(" ganger");
      display.display();
    }
}

void sjekkSensorer(){
  Serial.println(analogRead(fotpin));
  //Timeren er koblet til for å forsikre at en inngang ikke blir registrert som mange
  //Her kan man eksperimentere med flere timere, ulike intervaller og ulike sensorverdier :)
  if((millis()-timer2) >(intervall * 10)){
    //Sjekker om tiltsensoren har blitt aktivert
    if(digitalRead(tiltpin)==LOW){
      tiltAktiv += 1;
    }
    //Sjekker om det er mye lys i rommet
    if(analogRead(fotpin) >= 400){
      lysAktiv += 1;
    }
    timer2 = millis();
  }
}
