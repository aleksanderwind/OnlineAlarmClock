#include "sensors.h"

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

// Parameters for moving average lumen
const int numReadingsLumen  = 20;
float readingsLumen [numReadingsLumen];
int readIndexLumen  = 0;
float totalLumen = 0;

DHT* dhth;

void initSensors(DHT* dht, int photosens){
  dhth = dht;
  dht->begin(); // initialize the sensor
  pinMode(photosens,INPUT); // initialize LDR
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
  float temp = dhth->readTemperature();
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
  float humi = dhth->readHumidity();
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

int smoothLumen(const int ldrPin, const int R){
  /* Description
  Performs average of last N lumen readings (numReadingsLumen, defined in parameters at the beginning of doc).
  Light dependent resistor (LDR) should be connected to an analog pin and supplied with 5V. 
  Input [int]: Pin for LDR (ldrPin), Value of resistor connected to the LDR (R)
  Output [float]: Average luminosity in lumen of last N readings (lumenVal)
  
  Example: 
  >>> smoothLumen(A1)
    94
  */
  // Conversion rule
  float averageLumen;
  int photosensorVal = analogRead(ldrPin);
  float Vout = float(photosensorVal) * (5 / float(1023));// Conversion analog to voltage
  float Rldr = (R * (5 - Vout))/Vout; // Conversion voltage to resistance
  float lumenVal=500/(Rldr/1000); // Conversion resitance to lumen
  // subtract last reading
  totalLumen = totalLumen - readingsLumen[readIndexLumen];
  readingsLumen[readIndexLumen] = lumenVal;
  // add value to totalLumen:
  totalLumen = totalLumen + readingsLumen[readIndexLumen];
  // handle index
  readIndexLumen = readIndexLumen + 1;
  if (readIndexLumen >= numReadingsLumen) {
    readIndexLumen = 0;
  }
  // calculate the average:
  averageLumen = totalLumen / numReadingsLumen;

  return averageLumen;
}
