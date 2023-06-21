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
int TMPpin = A2;
int knappin = 8;
//Desse globale variablene kontrollerer skjermen
int cases = 3;    //Det totale tallet på cases
int kontroll = 1; //Tallet beskriver hvilken case man er i NÅ
//Timerere som kontrollerer når skjerm og knapp kan oppdaterest
long timer1 = 0;
long timer2 = 0;
long intervall = 1000;


void setup() {
  //Setter sensorpinnene som input
  pinMode(knappin, INPUT);
  pinMode(fotpin, INPUT);
  pinMode(TMPpin, INPUT);
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
  skjermKontroll(finnTemperatur(),finnLysstyrke());
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

void skjermKontroll(float temperatur, int lysstyrke){
  //Sjekker om timer 2 er gått ut
  if((millis()-timer2) > (intervall/2)){
    //Her visest en liten oppstartsmelding på skjermen
    if(kontroll == 1){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Trykk paa knappen for aa rullere");
      display.display();
    }
    //Dersom kontroll er 2 vil temperatur visast på skjermen
    else if(kontroll == 2){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Temperaturen er: ");
      display.print(temperatur, 1);
      display.display();
    }
    //Dersom kontroll er 3 vil lysstyrke visast på skjermen
    else if(kontroll == 3){
      display.clearDisplay();
      display.setCursor(0,10);
      display.print("Lysstyrken er: ");
      display.print(lysstyrke);
      display.display();
    }
    //Oppdater timer2
    timer2 = millis();
  }
}

int finnTemperatur(){
  //Leser av pinnen
  int read = analogRead(TMPpin);
  //Utregning baser på datablad
  float temp = read * (5000/1024.0);
  temp = (temp-500) /100;
  //Returnerer temperaturen
  return temp;
}

int finnLysstyrke(){
  //Leser av pinnen
  int lys = analogRead(fotpin);
  //Mapper dette som en prosent mellom 0 og 100(NB! bruk gjerne eksperimentelle verdier for din fotoresistor)
  lys = map(lys,1023,0,0,100);
  //Returnerer lysstyrken
  return lys;
}
