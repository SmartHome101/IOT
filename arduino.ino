#include "Adafruit_Keypad.h"
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);

#define ledOfStair   10
#define irOfStair2   11
#define irOfStair    12



Servo  lock ; 
#define Password_Length 8
char Data[Password_Length];
// Password
char Master[Password_Length] = "123A456";
char AnotherMaster[Password_Length] = "789C*0#";
 
 
// Counter for character entries
byte data_count = 0;

boolean firstIR ;  
boolean secondIR;

const byte ROWS = 4; // rows
const byte COLS = 4; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] =  {9, 8, 7, 6};//connect to the row pinouts of the keypad
byte colPins[COLS] =  {5, 4, 3, 2};//connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
  customKeypad.begin();
  lcd.begin();          
  lcd.clear();         
  lcd.backlight();
  lock.attach(13);
  
  pinMode(irOfStair, INPUT);
  pinMode(irOfStair2 , INPUT);
  pinMode(ledOfStair,OUTPUT);
}
 
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  
  customKeypad.tick();
   char key = ' ';
  while(customKeypad.available()){
    keypadEvent e = customKeypad.read();
    key = (char)e.bit.KEY;
    if(e.bit.EVENT == KEY_JUST_PRESSED){
    Data[data_count] = key;
//    Serial.print('*');
    lcd.setCursor(data_count, 1);
    lcd.print(Data[data_count]);
    data_count++;
    }
      // See if we have reached the password length
  if (data_count == Password_Length - 1) {
    
     lcd.clear();
 
    if (!strcmp(Data, Master)) {
        lcd.print("Correct");

  for (int pos = 120; pos >=0 ; pos -= 1) {
    lock.write(pos);              
    delay(15);                         }         
    }
    else if (!strcmp(Data , AnotherMaster)){
        lcd.print("Correct");
           for (int pos = 0; pos <= 120; pos += 1) { 
              lock.write(pos);              
              delay(15);             
  }   
        

    }
    else {
       lcd.print("Incorrect");
      delay(1000);
    }
 
    lcd.clear();
    clearData();
  }
  }

  delay(10);
 firstIR = digitalRead(irOfStair);
 secondIR = digitalRead(irOfStair2);
 Serial.println(firstIR);
 Serial.println(secondIR);
  if( firstIR ==0 || secondIR ==0){
    digitalWrite(ledOfStair,LOW);
    delay(2500);
  }
  else {
    digitalWrite(ledOfStair,HIGH);
  }

}
void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
