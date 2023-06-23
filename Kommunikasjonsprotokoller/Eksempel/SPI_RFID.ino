//RFID bibliotek og definisjoner
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
 
void setup() 
{
  //Start seriekommunikasjon
  Serial.begin(9600);
  //Start SPI dataoverføring
  SPI.begin();
  //Start mfrc522 
  mfrc522.PCD_Init();
  Serial.println("Approximate your card to the reader...");
  Serial.println();
}
void loop() 
{
  //Leit etter nye kort
  if ( !mfrc522.PICC_IsNewCardPresent() ){
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial() ){
    return;
  }
  //Leser av UID
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++){
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Respons : ");
  content.toUpperCase();
  //Dersom kortUID matcher med denne strengen
  if (content.substring(1) == "BD 31 15 2B"){ //Endre til din UID her!
    Serial.println("Tilgang tillatt");
    Serial.println();
    delay(3000);
  }
 else{
    Serial.println("Kort ikke godkjent");
    delay(3000);
  }
}

