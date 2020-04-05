#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <dht.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 4  //D2
#define RST_PIN 0 //D3

#define sensorLDR 5

#define RED D0
#define GREEN D8

MFRC522 mfrc522(SS_PIN, RST_PIN); 

int nilaiSensor;
int sensorSuhu = 2;
int counterA;
int counterB;
int counterC = 15;

dht DHT;

void setup() {
 
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  Wire.begin(3,1);              
  lcd.init(); 
  lcd.backlight();

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);

  pinMode(sensorLDR, INPUT);
}

void loop() {
   
    tempHumid();    
    bacaKartu();     
    lampuTaman();
}

void lampuTaman() {  
  nilaiSensor = digitalRead(sensorLDR);
  
  if(nilaiSensor == HIGH) {
    counterB = 15;
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
  
    if (counterA < 1){
     lcd.setCursor(0,1);
     lcd.print("                ");
    }
  
     else {    
       lcd.setCursor(0,1);
       lcd.print("Lampu menyala");
       counterA = counterA - 1; 
     } 
  }
    
  else {    
    counterA = 15;
      
    lcd.setCursor(0,1);
    lcd.print("                ");
     
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);

     if (counterB < 1){
     lcd.setCursor(0,1);
     lcd.print("                ");
    }
  
     else {    
     lcd.setCursor(0,1);
     lcd.print("Lampu mati");
     counterB = counterB - 1;
     }     
  }
}

void tempHumid() {
  DHT.read11(sensorSuhu);  
  lcd.setCursor(0,0);
  lcd.print("T:" + String(DHT.temperature, 0) + "\xDF");
  lcd.print("|H:" + String(DHT.humidity, 0) + "%");

  sprinkle();

}

void sprinkle() {
    if(DHT.humidity <= 50) {
        
      if (counterC < 1){
       lcd.setCursor(0,1);
       lcd.print("                ");
      }
        
       else {    
         lcd.setCursor(0,1);
         lcd.print("Sprinkle");
         counterC = counterC - 1;
       }
    
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);   
    delay(20);

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);   
    delay(80);

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);    
    delay(20);
  }
}

void bacaKartu() {   
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {    
      return;
    }
  
  String content= "";
  for (byte i = 0; i < mfrc522.uid.size; i++) { 
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  content.toUpperCase();
  
  if (content.substring(1) == "F9 CF BF 98") {      
    lcd.setCursor(0, 1);    
    scrollText(1, "Membuka gerbang...", 100, 16);    
    delay(500);

    lcd.setCursor(0, 1);
    lcd.print("Silahkan masuk");  

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
     
    lcd.print("               ");
    delay(3000);
  }
  
  else {
    lcd.setCursor(0, 1);    
    scrollText(1, "Membuka gerbang...", 100, 16);
    delay(500);

    lcd.setCursor(0, 1);
    lcd.print("Silahkan keluar");

    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
      
    lcd.print("               ");
    delay(3000);
  }
}

void scrollText(int row, String message, int delayTime, int lcdColumns)
{  
  for (int i = 0; i < lcdColumns; i++) {  
    message = " " + message;
  }
  
  message = message + " ";
  
  for (int pos = 0; pos < message.length(); pos++) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW); 
    delay(30); 

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    delay(30); 

    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    delay(30); 
    
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}
