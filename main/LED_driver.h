#ifndef LED_driver_h
#define LED_driver_h

/*
Header file created by: JKT
Authors for individual functions can be found in the LED_driver.cpp file.
*/

#include <Adafruit_NeoPixel.h>

long hexToDec(String hexString);

class LED {

private:

  int NUM_LEDS;

  int LED_PIN;

  Adafruit_NeoPixel* STRIP;

public:

  LED(int num_leds, int led_pin, Adafruit_NeoPixel * strip);

  void setLEDStrip(int r, int g, int b);

  void setLEDStripHex(long colorValue, float scaleBrightness = 1);

  void clear();
};

#endif  // LED_driver_h
