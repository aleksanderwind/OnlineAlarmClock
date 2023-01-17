#include "LED_driver.h"


long hexToDec(String hexString) {
  long decValue = 0;
  int nextInt;

  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt -= 48;
    if (nextInt >= 65 && nextInt <= 70) nextInt -= 45;
    if (nextInt >= 97 && nextInt <= 102) nextInt -= 87;
    nextInt = constrain(nextInt, 0, 15);
    decValue = (decValue * 16) + nextInt;
  }
  return decValue;
}

LED::LED(int num_leds, int led_pin, Adafruit_NeoPixel * strip) {

  NUM_LEDS = num_leds;

  LED_PIN = led_pin;

  STRIP = strip;

  STRIP->begin();
}

void LED::setLEDStrip(int r, int g, int b) {
  /*
   * Description
   * Takes three integers r, g and b and sets the led strip to the color specified by that three numbers.
   * 
   * INPUT: r,g,b number between 0 and 255
   * OUTPUT: none
   * 
   * Example:
   * setLEDStrip(255,0,0);
   * 
   * Sets the color of the LED strip to red.
  */

  //CURRENT_COLOR = "#" + String(r, HEX) + String(g, HEX) + String(b, HEX);

  for (int i = 0; i < NUM_LEDS; i++) {
    STRIP->setPixelColor(i, STRIP->Color(r, g, b));
  }
  STRIP->show();
}

void LED::setLEDStripHex(long colorValue, float scaleBrightness) {
  /*
   * Description
   * Takes a long value at sets the RGB strip to that color.
   * 
   * INPUT: long colorValue
   * OUTPUT: none
   * 
   * Example:
   * setLEDStrip(0xFF0000);
   * 
   * Sets the color of the LED strip to red.
  */

  int r, g, b;
  r = (int)((colorValue >> 16)*scaleBrightness);
  g = (int)(((colorValue & 0x00FF00) >> 8)*scaleBrightness);
  b = (int)((colorValue & 0xFF)*scaleBrightness);
  setLEDStrip(r, g, b);
}




void LED::clear() {
  STRIP->clear();
}
