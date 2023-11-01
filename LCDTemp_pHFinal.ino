#include <Wire.h>
#include "RTClib.h"
RTC_DS3231 rtc;
int curHr, curMin, curSec;
int hr1=6;
int min1=00;
int sec1=00;

int hr2=12;
int min2=00;
int sec2=00;

int hr3=18;
int min3=00;
int sec3=00;

int hr4=00;
int min4=00;
int sec4=00;

int inletSolenoid=13;

#include "DHT.h"
#define DHTPIN 2 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);


const int temp_pin= A0; 
const int heater_pin= 7;
const int fan_pin=5; 

//Set the range of the desire value
float minTemp=27, maxTemp=33; //Room Temp[28-33]


#include<LiquidCrystal.h>

LiquidCrystal LCD(8, 9, 4, 5, 6, 7); //RS, E, DB4, DB5. DB6, DB 7

//////////////////////pH Sensor//////////////////////
float calibration_value = 30.90-9.90;

int phval = 0;
unsigned long int avgval;
int buffer_arr[10],temp;
int PHpin=A0;
//////////////////pH Sensor///////////////////////////////

void setup(){
pinMode(inletSolenoid, OUTPUT);
if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

dht.begin();
LCD.begin (16, 2); 
pinMode(PHpin, INPUT);
 
pinMode(temp_pin, INPUT); 
pinMode(heater_pin, OUTPUT);   
pinMode(fan_pin, OUTPUT);   
digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 
LCD.print("Room Temp (C)"); 
LCD.setCursor(1,1);
LCD.print(minTemp); LCD.print("-"); LCD.print(maxTemp); 
  
  delay(1000); 
}

void loop(){

float SensorTemp = dht.readTemperature();
/*
float Eqv_volt, SensorTemp; 
  
Eqv_volt=analogRead(temp_pin)*5.0/1023; 
  SensorTemp=100.0*Eqv_volt-50.0; 
*/

///////////////////////////////pH///////////////////////
 
  for(int i=0;i<10;i++)
 {
 buffer_arr[i]=analogRead(PHpin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buffer_arr[i]>buffer_arr[j])
 {
 temp=buffer_arr[i];
 buffer_arr[i]=buffer_arr[j];
 buffer_arr[j]=temp;
 }
 }
 }
 avgval=0;
 for(int i=2;i<8;i++)
 avgval+=buffer_arr[i];
 float volt=(float)avgval*5.0/1024/6;
 float ph_act = -5.70 * volt + calibration_value;

/////////////////////////////pH///////////////////////

LCD.clear(); 
LCD.print("Temp Reading"); 
LCD.setCursor(2,1); 
LCD.print(SensorTemp); LCD.print("C");
digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW);  
delay(2000); 

LCD.clear();
LCD.print("pH Sensor");
LCD.setCursor(2,1); 
LCD.print(ph_act); LCD.print(" pH");
digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 
delay(3000);

  if (SensorTemp>=maxTemp){
  LCD.clear(); 
    LCD.print("Temp is higher"); 
    LCD.setCursor(0,1); LCD.print("Turn on the fan!"); 
    for (int i=0; i<=255; i++){
    analogWrite(fan_pin, i); 
    digitalWrite(heater_pin, LOW); 
    }
    delay(10000); 
    
    LCD.clear(); 
    LCD.print("Check Temp again"); 
    LCD.setCursor(0,1); 
    LCD.print("Turn off all"); 
    for (int i=255; i>=0; i--){
    analogWrite(fan_pin,i); 
    digitalWrite(heater_pin, LOW); 
    }
    delay(3000); 
  }
    else if (SensorTemp<minTemp){
    LCD.clear(); 
      LCD.print("Temp is LOWER"); 
      LCD.setCursor(0,1); LCD.print("Turn on Heater!"); 
      digitalWrite(heater_pin, HIGH); 
      digitalWrite(fan_pin, LOW);
    
    delay(10000); 
    
    LCD.clear(); 
    LCD.print("Check Temp again"); 
    LCD.setCursor(0,1); 
    LCD.print("Turn off all"); 
    delay(1000); 
    digitalWrite(heater_pin, LOW); 
    digitalWrite(fan_pin, LOW);
    LCD.clear(); 
    delay(3000);
  }
    else if (SensorTemp>minTemp && SensorTemp<maxTemp){
    LCD.clear(); 
     digitalWrite(heater_pin, LOW); 
    digitalWrite(fan_pin, LOW); 
      LCD.print("Awesome Day"); 
      LCD.setCursor(2,1); 
      LCD.print("Turn off all"); 
      
      LCD.clear();
      LCD.print("Loving Temp (C)"); 
      LCD.setCursor(1,1);
      LCD.print(minTemp); LCD.print("-"); LCD.print(maxTemp); 
       
     
    delay(2000); 
      LCD.clear(); 
    }
    else {
    LCD.clear(); 
     digitalWrite(heater_pin, HIGH); 
    digitalWrite(fan_pin, LOW); 
      LCD.print("Something went wrong"); 
      LCD.setCursor(2,1); 
      LCD.print("Circuit Failure"); 
      delay(1000); 
      LCD.clear(); 
    }
    delay(1000);
      digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 
 DateTime now = rtc.now();
   
    Serial.println("Current Date & Time: ");
    curHr=now.hour();
    curMin=now.minute();
    curSec=now.second();
    Serial.print(curHr, DEC);
    Serial.print(':');
    Serial.print(curMin, DEC);
    Serial.print(':');
    Serial.print(curSec, DEC);
    Serial.println();
   // delay(1000);
    LCD.clear();
    LCD.setCursor(3,0);
    LCD.print("TIME IST");
    LCD.setCursor(5,1);
    LCD.print(curHr);LCD.print(":");LCD.print(curMin);LCD.print(":");LCD.print(curSec);
    delay(2000);
    digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 

   if(curHr==hr1 && curMin==min1 && curSec==sec1 || curHr==hr2 && curMin==min2 && curSec==sec2 || curHr==hr3 && curMin==min3 && curSec==sec3){
    digitalWrite(inletSolenoid, HIGH);
    delay(60000);
    digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 
    }
    else {
     digitalWrite(inletSolenoid, LOW);
     digitalWrite(heater_pin, LOW); 
digitalWrite(fan_pin, LOW); 
      }
  }
