#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <Servo.h>


#define FIREBASE_HOST "smart-home-2f967-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "tMgdjSgoFv8ZV4NOjOoHEEF742ziQFzMoBi4qm1O"
#define WIFI_SSID "smarthome"
#define WIFI_PASSWORD "77777777"

#define trig1      D5 
#define echo1      D6

#define trig2      D7 
#define echo2      D8

#define enB        D1
#define in3        D2
#define in4        D3


#define enA        D4
#define in1        D9
#define in2        D10

#define sensorPin A0 
#define ledPin    D0 

String lightStatus;

int lightLevel;
int calibratedLightLevel;


int curtainStatus = 0;
int curtains = 0;
int garageVar  = 0;

FirebaseData firebaseData;

void setup() {
  Serial.begin(9600);
  pinMode(enB, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  pinMode(trig1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trig2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(ledPin, OUTPUT);
  

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
  Firebase.getInt(firebaseData,"HOME01/bedroom/on-off/curtain");
  curtainStatus = firebaseData.intData();
  Firebase.getString(firebaseData , "HOME01/bedroom/light/mode");
  lightStatus  =  firebaseData.stringData();
  Serial.println("garage value");
  int garageDistance = ultrasonic(trig1,echo1);
  Serial.println("danger value");
  int dangerDistance = ultrasonic(trig2,echo2);
  if (dangerDistance >=25 && dangerDistance <=30 ){
  Firebase.setString(firebaseData , "HOME01/Ultrasonic Value" ,String(dangerDistance) + " child is in danger");  
  } 
  else {
    Firebase.setString (firebaseData , "HOME01/Ultrasonic Value" ,String(dangerDistance) + " Safe zone");
  }
  



  lightLevel = analogRead(sensorPin);
  Serial.println("ldr value");
  Serial.print(lightLevel);
 

  calibratedLightLevel = map(lightLevel, 0 , 1023, 0, 255);

  Serial.print("\t");
  Serial.print("\n");
   delay(150);

  

  if ( garageDistance > 15 )
  {
    if (garageVar ==0){

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enA, 450);
    delay(1600);
 
  // Now turn off motors
 
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    
   
}
    garageVar = 1;
     
  }
  else 
  {
    if (garageVar ==1){

  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 450);
  delay(1600);
 
  // Now turn off motor
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

    }
    garageVar =0;
   
  }

  if (curtainStatus ==1){

    if (curtains ==0){
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
    analogWrite(enB, 450);
    delay(1700);
    Serial.println("hello curtain from the right side");
 
  // Now turn off motors
 
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
      
    }
    
   curtains =1;
   
}
    
  else{
    if ( curtains==1){
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 450);
  delay(1700);
 
  // Now turn off motor
 Serial.println("hello curtain from the left side");
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
    }
    curtains =0;
 
  }

  if (lightStatus == "low" && calibratedLightLevel < 100){
  analogWrite(ledPin, 100-calibratedLightLevel);}
  else if (lightStatus == "medium" && calibratedLightLevel < 200){
  analogWrite(ledPin, 200-calibratedLightLevel);}
  else if (lightStatus == "high"){
  analogWrite(ledPin, 255-calibratedLightLevel);}
  else if (lightStatus == "on"){
  analogWrite(ledPin, 255);}
  else{
  analogWrite(ledPin, 0);}


}
int ultrasonic(int trig , int echo){
  int distance=0,t=0;
  digitalWrite(trig,LOW);
  delayMicroseconds(5);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  t=pulseIn(echo,HIGH);
  distance=t/57;                                         //Distance = (Speed of Sound * Time/2) = t/(1/((350*0.0001)/2))
  Serial.println(distance);
  return distance ;
 
}
