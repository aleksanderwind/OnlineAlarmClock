#ifndef interface_h
#define interface_h

#include <Arduino.h>

/*  Control register codes (Refer to Table 2 of MAX7219 documentation) 
    Some elements have been left out.
*/
#define CTRL_DECODEMODE 0x09
#define CTRL_INTENSITY 0x0A
#define CTRL_SCANLIMIT 0x0B
#define CTRL_SHUTDOWN 0x0C
#define CTRL_DISPLAYTEST 0x0F
/* End of control register*/

#define ASCII_OFFSET 45   // Offset used to map char to index in asciiTableRef (see header file)
#define DISPLAY_LENGTH 8  // One module has 8 digits/segments

/*
An array of bytes which corresponds to characters on the 7-segment display. 
The array is offset, in such a manner that index = 0 corresponds to the character "-" on the display. 
The decimal value of "-" is 45, thus the ASCII_OFFSET is 45 as well. 
This way, subtracting the ASCII_OFFSET from the decimal value of a character one can index into the table below.
Not all characters can be display on the display. Q cannnot be displayed, and will instead display nothing.
Example: Display the letter L. (int)L = 76. 76 - ASCII_OFFSET = 31. 
The 31st index in the table below will display the letter L on the 7 segment. 
*/
const static byte asciiTableRef[] PROGMEM = {
  B00000001, B10000000, B00000000, B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000,
  B01111111, B01111011, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
  B00000000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B01111011,
  B00110111, B00110000, B00111000, B00000000, B00001110, B00000000, B00010101, B01111110,
  B01100111, B00000000, B00000101, B01011011, B00001111, B00111110, B00011100, B00000000,
  B00000000, B00111011, B00000000
};

class SegmentDriver {
private:
  int sendSPI(byte value, byte instruction);

  int MOSI;

  int CLK;

  int CS;

public:
  SegmentDriver(int MOSI, int CLK, int CS);
  int turnOn();
  int turnOff();
  int setBrightness(int value);
  int setScanLimit(int limit);
  int setChar(int place, char chr, bool dot);
  int setString(String string);
  int setClock(int hour, int minute);
  void clear();
};
#endif  // interface_h
