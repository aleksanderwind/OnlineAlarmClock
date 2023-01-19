#include "LED_driver.h"

/*
Function to convert from hex-string to dec.
Based on B. Rugg's (@benrugg) implementation from their GitHub Repository Arduino-Hex-Decimal-Conversion
url: https://github.com/benrugg/Arduino-Hex-Decimal-Conversion (2015)
Some changes have been made.
INPUT: hex values in a string
OUTPUT: long 
Example:
hexToDec("0000ff");

> 255

Author; JKT, AC
*/
long hexToDec(String hexString) {
  long decValue = 0; // Initialize final value
  int nextInt;

  // For each character in the string:
  for (int i = 0; i < hexString.length(); i++) {
    nextInt = int(hexString.charAt(i)); // The decimal value of the character is stored in nextInt
    // If the decimal value of the character is between 48 and 57, it is a number between 0 and 9
    if (nextInt >= 48 && nextInt <= 57) nextInt -= 48; // By subtracting 48 the actual number is found

    // If the decimal value of the character is between 65 and 70, it is a capital letter between A and F. 
    if (nextInt >= 65 && nextInt <= 70) nextInt -= 56; // Subtracting 56, a number between 10 and 15 is achieved

    // If the decimal value of the character is between 97 and 102, it is a lower case letter between a and f. 
    if (nextInt >= 97 && nextInt <= 102) nextInt -= 87; // Subtracting 87, a number between 10 and 15 is achieved

    nextInt = constrain(nextInt, 0, 15); // We make sure the value is mapped between 0 and 15.

    decValue = (decValue * 16) + nextInt; // For each time we get a new number, we bit shift with 4 (eq. to multiplying with 16)
  }
  return decValue;
}

/*
Class constructor. Takes the inputs and stores them in private variables.
Author: JKT
*/
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
   * 
   * Author: AC
  */

  //CURRENT_COLOR = "#" + String(r, HEX) + String(g, HEX) + String(b, HEX);

  r = (r<0) ? 0 : r;
  g = (g<0) ? 0 : g;
  b = (b<0) ? 0 : b;

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
   * 
   * Author: AC
  */

  int r, g, b;
  r = (int)((colorValue >> 16)*scaleBrightness);
  g = (int)(((colorValue & 0x00FF00) >> 8)*scaleBrightness);
  b = (int)((colorValue & 0xFF)*scaleBrightness);
  setLEDStrip(r, g, b);
}


/*
 Clear the LED strip using the NeoPixel clear() function.
 Author: JKT 
*/
void LED::clear() {
  STRIP->clear();
}
