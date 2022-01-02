#include <DS3231.h>
#include <SD.h>
#include <SPI.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
RTClib myRTC;
Adafruit_BME280 bme;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();


  if (!SD.begin(2)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

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
  dataString += String(bme.readTemperature());
  dataString += ",";
  dataString += String(bme.readHumidity());
  dataString += ",";
  dataString += String(bme.readPressure()/100.0F);
  dataString += ",";
  dataString += String(bme.readAltitude(SEALEVELPRESSURE_HPA));
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
