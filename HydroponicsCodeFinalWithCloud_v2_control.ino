#include <OneWire.h>
#include <DallasTemperature.h>
#define SENSOR_PIN  4 
OneWire oneWire(SENSOR_PIN);
DallasTemperature DS18B20(&oneWire);
float tempC; 

const int pump=11;
////////////////////////////////////////////////TDS/////////////////////////
int TdsSensorPin= 36;
int tdsValue;
/*
#define VREF 3.3              // analog reference voltage(Volt) of the ADC
#define SCOUNT  30            // sum of sample point

int analogBuffer[SCOUNT];     // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;

float temperature = 25;       // current temperature for compensation

// median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}
*/
///////////////////////////////////////TDS////////////////////////////


///////////////////////////////////////////////////////////////////////
#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "OnePlus1";   // your network SSID (name) 
const char* password = "anupsharma";   // your network password

WiFiClient  client;

unsigned long myChannelNumber = 1768464;
const char * myWriteAPIKey = "P4DO6MOC2ZHPNEIE";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

//////////////////////////////////////////////////////////////////////
const int trigPin = 19;
const int echoPin = 5;
#define SOUND_SPEED 0.034
long duration;
float distanceCm;

void setup() {
  Serial.begin(115200); 
  DS18B20.begin(); 
//  Serial.begin(115200); 
  pinMode(TdsSensorPin, INPUT);  
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  
  pinMode(pump, OUTPUT);
//  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);

}

void loop() {
  DS18B20.requestTemperatures();       
  tempC = DS18B20.getTempCByIndex(0); 
tdsValue=analogRead(TdsSensorPin);
//delay(100);
//Serial.print(TdsSensorPin);
  /*
  static unsigned long analogSampleTimepoint = millis();
  if(millis()-analogSampleTimepoint > 40U){     //every 40 milliseconds,read the analog value from the ADC
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if(analogBufferIndex == SCOUNT){ 
      analogBufferIndex = 0;
    }
  }   
  static unsigned long printTimepoint = millis();
  if(millis()-printTimepoint > 800U){
    printTimepoint = millis();
    for(copyIndex=0; copyIndex<SCOUNT; copyIndex++){
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
   
      // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 4096.0;
      
      //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
      float compensationCoefficient = 1.0+0.02*(temperature-25.0);
      //temperature compensation
      float compensationVoltage=averageVoltage/compensationCoefficient;
      
      //convert voltage value to tds value
      tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5;
*/
 ///////////////////////////////////////////////////////////////////////////////////////////
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
      //Serial.print("voltage:");
      //Serial.print(averageVoltage,2);
      //Serial.print("V   ");
///////////////////////////////////////////////////////////////////////

if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

     // set the fields with the values
    ThingSpeak.setField(1, tdsValue);
    ThingSpeak.setField(2, tempC);
    ThingSpeak.setField(3, distanceCm);
    
    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
////////////////////////////////////////////////////////////////////////

if (tdsValue>50 && tdsValue<100 && distanceCm>15){
  digitalWrite(pump, HIGH);
  delay(6000);
  }
else{
  digitalWrite(pump, LOW);
  }
 
////////////////////////////////////////////////////////////////////////
  Serial.print("TDS Value:");
  Serial.print(tdsValue);
  Serial.println("ppm");
  Serial.print("Temperature: ");
  Serial.print(tempC);    
  Serial.println("°C");
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
    delay(1000);

 

}
