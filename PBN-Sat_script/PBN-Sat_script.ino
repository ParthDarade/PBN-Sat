

/* 
      *************************************************************
                   --------------------------------------
                              PBN Sat mission
                   --------------------------------------
      This code is written for testing and devlopment of PBN Sat Demo
      written by: Parth Darade
      **************************************************************
*/
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <Adafruit_MQTT_FONA.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <TinyGPS++.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define R_SSID       "Nokia 2.2"
#define PASS       "123456789"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Parth_Darade"
#define AIO_KEY         "aio_bORz486O4EMRe5AzZAZTA2rDZaFm"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish Hum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
Adafruit_MQTT_Publish Press = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pressure");
Adafruit_MQTT_Publish AirQua = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/air-quality");
Adafruit_MQTT_Publish GPSlatlng = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/gpslatlng");
Adafruit_MQTT_Publish Time = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/time");
Adafruit_MQTT_Publish Alt = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/alt");



Adafruit_BME280 bme;

int mq135Pin = A0;



TinyGPSPlus gps;
static const int GPS_Tx = D2;
static const int GPS_Rx = D3;
static const int GPSBaud=9600;

SoftwareSerial GPSS(GPS_Rx, GPS_Tx);




void setup() {
  
  // starting serial communication
 Serial.begin(9600);

 GPSS.begin(GPSBaud);

 //checking status of BME 280
 bool BME_status = bme.begin(0x76);
 if (!BME_status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }



pinMode (mq135Pin,INPUT);
  
  Serial.println("setup check complete");

}


void loop() {
  logBMEData();
  logAirQ();
  
  logGPS();
  Serial.println();
  delay(5000);
}


 void logBMEData(){
     
     Temp.publish(bme.readTemperature());
    
     Hum.publish(bme.readHumidity());
   
    alt.publish(bme.readAltitude(SEALEVELPRESSURE_HPA));
  
   
    Press.publish(bme.readPressure() / 100.0F);
 
  
   }


   void logAirQ(){
     int index = averageAnalogRead(mq135Pin);
     Serial.print("AirQ index ");
     Serial.println(index);
     int AirQPercentage= map(index,100,450,0,100);
     Serial.print("percentage: ");
     Serial.println(AirQPercentage);
   }

   
   void logGPS(){
     while (GPSS.available() > 0)
    if (gps.encode(GPSS.read()))
      Serial.print(F("Location: "));
  displayGPS();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
   }
   void displayGPS(){
    if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.month());
    
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
   }

    int averageAnalogRead(int pinToRead)
  {
    byte numberOfReadings = 8;
    unsigned int runningValue = 0; 
      for(int x = 0 ; x < numberOfReadings ; x++)
         runningValue += analogRead(pinToRead);
         runningValue /= numberOfReadings;
    return(runningValue);  
  }
  float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
   {
     return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
   } 
