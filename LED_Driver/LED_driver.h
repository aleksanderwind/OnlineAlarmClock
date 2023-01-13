#ifndef LED_driver_h
#define LED_driver_h

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

  void setLEDStripHex(long hex);

  void clear();
};

#endif  // LED_driver_h