/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-temperature-humidity-sensor
 */

#include "DHT.h"

// Parameters for smooth temp
const int numReadingsTemp = 5;
float readingsTemp [numReadingsTemp];
int readIndexTemp = 0;
float totalTemp = 0;

// Parameters for smooth humidity
const int numReadingsHumi = 5;
float readingsHumi [numReadingsHumi];
int readIndexHumi = 0;
float totalHumi = 0;


const int DHTPIN = 13;
const int DHTTYPE = DHT11;

DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(9600);
  dht.begin(); // initialize the sensor
}

void loop() {
    Serial.print("Humidity: ");
    Serial.print(dht.readHumidity());
    Serial.println("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(dht.readTemperature());
    Serial.println("°C");


    Serial.print("Humidity: ");
    Serial.print(smoothHumiDHT());
    Serial.println("%");

    Serial.print("  |  "); 

    Serial.print("Temperature: ");
    Serial.print(smoothTempDHT());
    Serial.println("°C");
    delay(2000);
}

float smoothTempDHT(){
  /* Description
  Performs average of last N temperature readings (numReadingsTemp, defined in parameters at the beginning of doc).
  DHT sensor should be connected to a digital port.
  Input []: None
  Output [float]: Average temperature in degrees Celsius of last N readings
  
  Example: 
  >>> smoothTempDHT()
    22.4
  */
  float averageTemp;
  // subtract last reading
  totalTemp = totalTemp - readingsTemp[readIndexTemp];
  // read temp and add to array
  float temp = dht.readTemperature();
  readingsTemp[readIndexTemp] = temp;
  // add value to totalTemp:
  totalTemp = totalTemp + readingsTemp[readIndexTemp];
  // handle index
  readIndexTemp = readIndexTemp + 1;
  if (readIndexTemp >= numReadingsTemp) {
    readIndexTemp = 0;
  }
  // calculate the average:
  averageTemp = totalTemp / numReadingsTemp;
  return averageTemp;
}

float smoothHumiDHT(){
 /* Description
  Performs average of last N humidity readings (numReadingsHumi, defined in parameters at the beginning of doc).
  DHT sensor should be connected to a digital port.
  Input []: None
  Output [float]: Average humidity of last N readings in percentage 
  
  Example: 
  >>> smoothHumiDHT()
    39.0
  */
  float averageHumi;
  // subtract last reading
  totalHumi = totalHumi - readingsHumi[readIndexHumi];
  // read humidity and add to array
  float humi = dht.readHumidity();
  readingsHumi[readIndexHumi] = humi;
  // add new value to totalHumi:
  totalHumi = totalHumi + readingsHumi[readIndexHumi];
  // handle index
  readIndexHumi = readIndexHumi + 1;
  if (readIndexHumi >= numReadingsHumi) {
    readIndexHumi = 0;
  }
  // calculate the average:
  averageHumi = totalHumi / numReadingsHumi;

  return averageHumi;
}
