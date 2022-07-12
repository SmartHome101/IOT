#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h> 


#define FIREBASE_HOST "smart-home-2f967-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "tMgdjSgoFv8ZV4NOjOoHEEF742ziQFzMoBi4qm1O"
#define WIFI_SSID "smarthome"
#define WIFI_PASSWORD "77777777"

#define DHT11_PIN D6
#define DHTTYPE DHT11

#define fan1Pin D7
Servo windowServo ; 
Servo doorServo ;

int lcdStatus = 0 ; 
int doorStatus = 0 ; 
int windowStatus = 0;
int fan1Status = 0 ; 
String RainStatus ;
int doorVar = 0;
int windowVar = 0;

FirebaseData firebaseData;
LiquidCrystal_I2C lcd(0x27,16,2);                              

DHT dht(DHT11_PIN ,DHTTYPE);         

void setup() {
  Serial.begin(9600);
  pinMode(DHT11_PIN, INPUT);
  pinMode(fan1Pin , OUTPUT);
  lcd.begin();          
  lcd.clear();         
  lcd.backlight();
 
  windowServo.attach(D5);  
  doorServo.attach(D3);
  windowServo.write(0);
  doorServo.write(0);

   
  
  while (!Serial ) continue ; 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
 
  Serial.println();
  Serial.println("Connected.");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 

}

void loop() {
 
  Firebase.getInt(firebaseData,"HOME01/living room/on-off/window");
   windowStatus =firebaseData.intData();
  Firebase.getInt(firebaseData,"HOME01/door");
  doorStatus   = firebaseData.intData();
  Firebase.getInt(firebaseData , "HOME01/living room/on-off/LCD display");
  lcdStatus = firebaseData.intData();  
  Firebase.getInt(firebaseData , "HOME01/living room/on-off/fan");
  fan1Status = firebaseData.intData(); 
  Firebase.getString(firebaseData ,"HOME01/Rain Status");
  RainStatus = firebaseData.stringData(); 

if( RainStatus== "No Rain"){
  if (windowStatus == 1 )
  {
   if(windowVar ==0){
      for (int i = 120;i>=0 ; i--){
     windowServo.write(i);
     delay(15);   
      }
      
    }
    windowVar = 1;
    
    Serial.println("window opened");
  }
  else if (windowStatus == 0)
  {
   if(windowVar ==1){
      for (int i = 0;i<=120 ; i++){
     windowServo.write(i);
     delay(15);   
      }
      
    }
    windowVar = 0;
    Serial.println("window closed");
  }}
  //there are rain  close the window 
  else{  
   if(windowVar ==1){
      for (int i = 0;i<=120 ; i++){
     windowServo.write(i);
     delay(15);   
      }
      
    }
    windowVar = 0;
    Serial.println("window closed");
  }
 if (doorStatus == 1 )
  {
   if(doorVar ==0){
      for (int i = 120;i>=0 ; i--){
     doorServo.write(i);
     delay(15);   
      }   
    }
    doorVar = 1;
    
    Serial.println("door open");
  }
  else if (doorStatus == 0)
  {
   if(doorVar ==1){
      for (int i = 0;i<=120 ; i++){
     doorServo.write(i);
     delay(15);   
      }
      
    }
    doorVar = 0;
    Serial.println("door closed");
  }
 if (lcdStatus ==1 ){
            lcdDisplay();
            Serial.println("lcd now ");
           
  }
  else {
    lcdClear();
    Serial.println("clear lcd ");
}

 if (fan1Status == 1 ) {
     digitalWrite(fan1Pin , LOW);
     Serial.println("fan1 ON");
  }  else {
   digitalWrite( fan1Pin, HIGH); 
      Serial.println("fan1 OFF");
  }
}
void lcdDisplay() {
   lcd.noCursor();                           //no display the cursor
   delay(2000);
  Serial.println(dht.readTemperature());   
  Serial.println(dht.readHumidity()); 
  lcd.home();                                //set the cursor on the upper left side of LCD
  //start print temperature and humidity on LCD
  lcd.print("Temp: ");
  lcd.print(dht.readTemperature());
  lcd.print((char)223);                      //the ASCII code of degree symbol
  lcd.print(" c  ");
  lcd.setCursor(0,1);                        //set the cursor on the starting of the second row    " get down line "
  lcd.print("Humidity: " );
  lcd.print(dht.readHumidity());
  lcd.print("%");
}

void lcdClear(){
   lcd.noCursor();                            
   lcd.home();                                
   lcd.clear();
}
