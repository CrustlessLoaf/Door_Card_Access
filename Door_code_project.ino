//Door Code Project
#include <SPI.h>
#include <MFRC522.h>
#include <IRremoteInt.h>
#include <IRremote.h>
#include<Servo.h>
Servo myservo;
int RECV_PIN = 2;
int Solenoid = 7;
int Door = 3;
int pos = 0;

IRrecv irrecv(RECV_PIN);
decode_results results; 
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();
  pinMode(Solenoid, OUTPUT);
  pinMode(Door,OUTPUT);
  myservo.attach(5);
  irrecv.enableIRIn();

}
void loop() 
{ if (irrecv.decode(&results))
    {
      if(results.value == 0xFF30CF){digitalWrite(Solenoid, HIGH);delay(500);}
      else if (results.value == 0xFF18E7){digitalWrite(Solenoid, LOW);}
      else if (results.value == 0xFF10EF){Serial.println("open");digitalWrite(Door, HIGH);for (pos = 0; pos < 90; pos ++){
      myservo.write(pos);
      delay(15);
    }}
      else if (results.value == 0xFF38C7){Serial.println("closed");digitalWrite(Door, LOW);for (pos = 89;pos > 0;pos--){
      myservo.write(pos);
      delay(15);
    }}
     Serial.println(results.value, HEX);
     irrecv.resume(); // Receive the next value
    }
 // Look for new cards
 if ( ! mfrc522.PICC_IsNewCardPresent()) 
  // Select one of the cards
  return;
  else if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if ((content.substring(1) == "AE 58 A2 79")||(content.substring(1) == "3E ED 31 9B")||(content.substring(1) == "50 ED 30 A8"))//change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    delay(1000);
    digitalWrite(Solenoid, HIGH);
    for (pos = 0; pos < 90; pos ++){
      myservo.write(pos);
      delay(15);
    }
    Serial.println("Servo moved");
    delay(3000);
    for (pos = 89;pos > 0;pos--){
      myservo.write(pos);
      delay(15);
    }
  }
 else   {
    Serial.println(" Access denied");
    delay(3000);
  }}

