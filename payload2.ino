#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <DS3231.h>
#include <SD.h>
#include <SPI.h>

#define SEALEVELPRESSURE_HPA (1013.25)

#define DHTPIN 3
#define DHTTYPE DHT22

RTClib myRTC;

Adafruit_BMP085 bmp;
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  //SD begiin
  if (!SD.begin(2)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
//BMP begin
   if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
  dht.begin();

    }



void loop() {
  // put your main code here, to run repeatedly:
  String dataString = "";
  delay(3000);
  DateTime now = myRTC.now();
  dataString += String(now.year(), DEC);
  dataString += "/";
  dataString += String(now.month(), DEC);
  dataString += "/";
  dataString += String(now.day(), DEC);
  dataString += " ";
  dataString += String(now.hour(), DEC);
  dataString += ":";
  dataString += String(now.minute(), DEC);
  dataString += ":";
  dataString += String(now.second(), DEC);
  dataString += ",";
  dataString += String(dht.readHumidity());
  dataString += ",";
  dataString += String(dht.readTemperature());
  dataString += ",";
  dataString += String(bmp.readPressure()/100.0F);
  dataString += ",";
  dataString += String(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  dataString += ",";
  dataString += String(map(analogRead(A0),0,1023,0,500));
  dataString += ",";
  dataString += String(map(analogRead(A1),0,1023,0,500));
  dataString += "#";
  
   
   //file writing
   File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
  }


  
}
