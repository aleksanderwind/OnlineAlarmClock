/*
   Created by ArduinoGetStarted.com

   This example code is in the public domain

   Tutorial page: https://arduinogetstarted.com/tutorials/arduino-temperature-humidity-sensor
*/

#ifndef DHTsensor_h
#define DHTsensor_h

#include <DHT.h>

struct data {
  float temperature;
  float humidity;
  float lightLevel;
};

/*
#define DHTPIN 13
#define DHTTYPE DHT22
*/
class DHTsensor
{
  private:
    // Parameters for smooth temp
    const int numReadingsTemp = 5;
    float readingsTemp[5];
    int readIndexTemp;
    float totalTemp;

    // Parameters for smooth humidity
    const int numReadingsHumi = 5;
    float readingsHumi[5];
    int readIndexHumi;
    float totalHumi;

    // Parameters for smooth lumen reading
    const int numReadingsLumen  = 20;
    float readingsLumen [20];
    int readIndexLumen;
    float totalLumen;
    int ldrpin;
    const int R = 220;
    
   public:
    DHT dht;
    DHTsensor (int , int );
    //DHTsensor(int dhtPin) : dht(dhtPin, DHT22) {}
    float smoothTempDHT();
    float smoothHumiDHT();
    float smoothLumen();
    void dhtBegin();
};

//DHTsensor::DHTsensor(int dhtPin):dht(dhtPin, DHT22){}

#endif
