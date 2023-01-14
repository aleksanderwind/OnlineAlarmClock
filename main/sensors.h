#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <Arduino.h>
#include <DHT.h>

void initSensors(DHT* dht, int photosens);
float smoothTempDHT();
float smoothHumiDHT();
int smoothLumen(const int ldrPin, const int R);

#endif
