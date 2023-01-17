/*
   DHTsensor.cpp
   Library containing DHT sensor functionality.
*/

#include "Arduino.h"
#include <DHT.h>
#include "DHTsensor.h"

DHTsensor::DHTsensor(int dhtPin, int ldrPin):dht(dhtPin, DHT11) //constructor for DHTsensor object, along with creating an object of the DHT class
{
  Serial.println(dhtPin);
  // Parameters for smooth temp
  readIndexTemp = 0;
  totalTemp = 0;

  // Parameters for smooth humidity
  readIndexHumi = 0;
  totalHumi = 0;

  // Parameters for smooth lumen reading
  readIndexLumen  = 0;
  totalLumen = 0;
  ldrpin = ldrPin;
}

void DHTsensor::dhtBegin()
{
  pinMode(ldrpin,INPUT);
  dht.begin();
}

float DHTsensor::smoothTempDHT() {
  /* Description
    Performs average of last N temperature readings (numReadingsTemp, defined in parameters at the beginning of doc).
    DHT sensor should be connected to a digital port.
    Input []: None
    Output [float]: Average temperature in degrees Celsius of last N readings

    Example:
    >>> smoothTempDHT()
    22.4
  */
  float avgTmp = 0;
  
  // subtract last reading
  totalTemp = totalTemp - readingsTemp[readIndexTemp];
  
  // read temp and add to array
  float temp = dht.readTemperature();
  readingsTemp[readIndexTemp] = temp;
  
  // add value to totalTemp:
  totalTemp = totalTemp + readingsTemp[readIndexTemp];
  
  // handle index
  readIndexTemp = readIndexTemp + 1;
  
  if (readIndexTemp >= numReadingsTemp) 
  {
    readIndexTemp = 0;
  }
  
  // calculate and return the average: 
  avgTmp = totalTemp / numReadingsTemp;
  return avgTmp;
}

float DHTsensor::smoothHumiDHT() {
  /* Description
    Performs average of last N humidity readings (numReadingsHumi, defined in parameters at the beginning of doc).
    DHT sensor should be connected to a digital port.
    Input []: None
    Output [float]: Average humidity of last N readings in percentage

    Example:
    >>> smoothHumiDHT()
     39.0
  */

  float avgHum;

  // subtract last reading
  totalHumi = totalHumi - readingsHumi[readIndexHumi];

  // read humidity and add to array
  float humi = dht.readHumidity();
  readingsHumi[readIndexHumi] = humi;

  // add new value to totalHumi:
  totalHumi = totalHumi + readingsHumi[readIndexHumi];

  // handle index
  readIndexHumi = readIndexHumi + 1;

  if (readIndexHumi >= numReadingsHumi)
  {
    readIndexHumi = 0;
  }
  
  // calculate and return the average:
  avgHum = totalHumi / numReadingsHumi;
  return avgHum;
}

float DHTsensor::smoothLumen(){
    //Serial.print("lumnDebug ");

  /* Description
  Performs average of last N lumen readings (numReadingsLumen, defined in parameters at the beginning of doc).
  Light dependent resistor (LDR) should be connected to an analog pin and supplied with 3.3V. 
  Input [int]: Pin for LDR (ldrPin), Value of resistor connected to the LDR (R)
  Output [float]: Average luminosity in lumen of last N readings (lumenVal)
  
  Example: 
  >>> smoothLumen(A1)
    94
  */
  float avgLum;
  
  // Conversion rule
  int photosensorVal = analogRead(ldrpin);
  float Vout = float(photosensorVal) * (float(3.3) / float(1023));// Conversion analog to voltage
  float Rldr = (R * (float(3.3) - Vout))/Vout; // Conversion voltage to resistance
  float lumenVal=500/(Rldr/1000); // Conversion resitance to lumen
  // subtract last reading
  totalLumen = totalLumen - readingsLumen[readIndexLumen];
  readingsLumen[readIndexLumen] = lumenVal;
  // add value to totalLumen:
  totalLumen = totalLumen + readingsLumen[readIndexLumen];
  // handle index
  readIndexLumen = readIndexLumen + 1;
  if (readIndexLumen >= numReadingsLumen) 
  {
    readIndexLumen = 0;
  }
  // calculate the average:
  avgLum = totalLumen / numReadingsLumen;
  return avgLum;
}
