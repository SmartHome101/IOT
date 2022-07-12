#include <DS3232RTC.h>     
DS3232RTC myRTC;


#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>


#define FIREBASE_HOST "smart-home-2f967-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "tMgdjSgoFv8ZV4NOjOoHEEF742ziQFzMoBi4qm1O"
#define WIFI_SSID "smarthome"
#define WIFI_PASSWORD "77777777"

#define ledPin1    D0
#define ledPin2    D6               //Reception
#define ledPin3    D9               //Living_room
#define ledPin4    D3

#define FLAME      D5
#define ALARM      D4


#define p           D8                 //power pin for rain sensor
#define Do          D7                 //define the pin 4 for the sensor digital output (0 "it is rainning"  or  1 "it isn't rainning").


#define AnalogInput  A0


int led1Status = 0;
int led2Status = 0;
int led3Status = 0;
int led4Status = 0;

int fire =0;
int s ; 
int d ; 
int c =0;
int H ;
int M ;
int S ;
int D ;
int Mo ;
int Y ;

                                 
FirebaseData firebaseData;

void setup() {
  
  Serial.begin(9600);
  myRTC.begin();
 pinMode(ledPin1, OUTPUT);
 pinMode(ledPin2 , OUTPUT);
 pinMode(ledPin3 , OUTPUT);
 pinMode(ledPin4 , OUTPUT);
 pinMode(FLAME, INPUT);
 pinMode(ALARM, OUTPUT);
 pinMode( Do , INPUT);
 pinMode( AnalogInput, INPUT);  
 pinMode(p,OUTPUT);
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

  tmElements_t tm;  
  myRTC.read(tm);
 fire = flame(); 
  s=readSensor();
  d =detdensity(); 
 Serial.println(d);
 if(fire ==1){
//    tmElements_t tm;  
//    myRTC.read(tm);

Firebase.setString(firebaseData , "HOME01/kitchen/fire" , "warning Fire at "+
String(tm.Hour) +
':' +
String(tm.Minute)+
':'+String(tm.Second)+
" On "+
String(tm.Day)+
"/"+
String(tm.Month)+
"/"+
String(tmYearToCalendar(tm.Year)));
Serial.println("setting fire on firebase now ");
    if( c == 0){ 
      H = tm.Hour ;
      M = tm.Minute ;
      S = tm.Second ;
      D = tm.Day ;
      Mo= tm.Month ;
      Y = tmYearToCalendar(tm.Year) ;
     c = 1;} 
      Serial.println("setting fire on firebase now "); 
    }
    else {
      Firebase.setString(firebaseData, "HOME01/kitchen/fire", "There's no fire !!");
     if( c == 1){
            int H2=tm.Hour;
            int M2=tm.Minute;
            int S2=tm.Second; 
      if(S2<S){
       M2--;
       S2 +=60;}
      if(M2<M){
       H2--;
       M+=60;}
      if(H2<H){
       H2 +=12;}
     int dh = H2 - H ;
     int dm = M2 - M ;
     int ds = S2 - S ;
     Firebase.pushString(firebaseData , "HOME01/kitchen/history of fire" , "Fire happened at  " +
       String(H) + ':' + 
       String(M) + ':' +  
       String(S) + "  on "  +
       String(D) + "/" +
       String(Mo)+ "/"  +
       String(Y)+ "/   for "  + 
       String(dh)+ ':' + 
       String(dm)+ ':' +
       String(ds) 
);
}
     c = 0;} 
    
    
 if(s ==0){
  Firebase.setString(firebaseData,"HOME01/Rain Status" , "Rain");
   if(d >=250 &d<=333){
           Firebase.setString(firebaseData,"HOME01/Rain level" , "Heavy"); 
         }
     else if(d>=334 & d<=340){

          Firebase.setString(firebaseData ,"HOME01/Rain level" , "Moderate");
    
         }
      else if(d>= 341 & d<=360){
          Firebase.setString(firebaseData,"HOME01/Rain level" , "Light");
      
          }
   }
   else{
    Firebase.setString(firebaseData,"HOME01/Rain Status" , "No Rain");
         Firebase.setString(firebaseData,"HOME01/Rain level" , " "); 
   }
 Firebase.getInt(firebaseData , "HOME01/bathroom/on-off/light");
  led1Status = firebaseData.intData();

if (led1Status == 1 ) {
     digitalWrite(ledPin1 , LOW);
     Serial.println("led1 on");
  }  else {
   digitalWrite( ledPin1, HIGH); 
      Serial.println("led1 off ");
  }

   Firebase.getInt(firebaseData , "HOME01/reception/on-off/light");
   led2Status = firebaseData.intData();

   if (led2Status == 1 ) {
     digitalWrite(ledPin2 , LOW);
     Serial.println("led2 on");
  }  else {
   digitalWrite( ledPin2, HIGH); 
      Serial.println("led2 off ");
  }

   Firebase.getInt(firebaseData , "HOME01/living room/on-off/light");
   led3Status = firebaseData.intData();

   if (led3Status == 1 ) {
     digitalWrite(ledPin3 , LOW);
     Serial.println("led3 on");
  }  else {
   digitalWrite( ledPin3, HIGH); 
      Serial.println("led3 off ");
  }

   Firebase.getInt(firebaseData , "HOME01/kitchen/on-off/light");
   led4Status = firebaseData.intData();

     if (led4Status == 1 ) {
     digitalWrite(ledPin4 , LOW);
     Serial.println("led4  on");
  }  else {
   digitalWrite( ledPin4, HIGH); 
      Serial.println("led4  off ");
  }
}


boolean flame(){
  int fire = digitalRead(FLAME);


  if( fire == HIGH)
  {
    digitalWrite(ALARM,HIGH);
  }else{
    digitalWrite(ALARM,LOW);
  }
  delay(200);
  return fire ; 
}

int readSensor(){
  digitalWrite( p , HIGH);   // turn ON the sensor
  delay(10);                 //to allow the sensor get the right value 
  int v=digitalRead(Do);      //read the output of sensor  
  digitalWrite( p , LOW);   // turn ON the sensor
  return v;   
  }



int detdensity(){
  digitalWrite( p , HIGH);   // turn ON the sensor
  delay(10);                 //to allow the sensor get the right value int 
  int v=analogRead(AnalogInput);   //read the output of sensor  
  digitalWrite( p , LOW);   // turn ON the sensor
  return v;
 }
