#ifndef DHTsensor_h
#define DHTsensor_h

/*
Header file created by: AW
Authors for individual functions can be found in the themes.cpp file.
*/

#include <DHT.h>

struct data {
  float temperature;
  float humidity;
  float lightLevel;
};

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
    float smoothTempDHT();
    float smoothHumiDHT();
    float smoothLumen();
    void dhtBegin();
};

#endif
