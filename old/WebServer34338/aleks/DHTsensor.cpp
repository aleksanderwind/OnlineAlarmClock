/*
   DHTsensor.cpp
   Library containing DHT sensor functionality.
*/

#include "Arduino.h"
#include <DHT.h>
#include "DHTsensor.h"

DHTsensor::DHTsensor(int dhtPin):dht(dhtPin, DHT11)
{
  // Parameters for smooth temp
  //numReadingsTemp = 5;
  //float readingsTemp [numReadingsTemp];
  readIndexTemp = 0;
  totalTemp = 0;

  // Parameters for smooth humidity
  //numReadingsHumi = 5;
  //float readingsHumi [numReadingsHumi];
  readIndexHumi = 0;
  totalHumi = 0;
}

void DHTsensor::dhtBegin()
{
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
  float avgTmp;
  
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
