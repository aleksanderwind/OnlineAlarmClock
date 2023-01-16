#ifndef DHTsensor_h
#define DHTsensor_h

#include <DHT.h>

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
    int smoothLumen();
    void dhtBegin();
};

#endif
