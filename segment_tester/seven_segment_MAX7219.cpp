#include "seven_segment_MAX7219.h"

/* Control register (Table 2) */
#define CTRL_DECODEMODE 0x09
#define CTRL_INTENSITY 0x0A
#define CTRL_SCANLIMIT 0x0B
#define CTRL_SHUTDOWN 0x0C
#define CTRL_DISPLAYTEST 0x0F

#define ASCII_OFFSET 45
#define DISPLAY_LENGTH 8


SegmentDriver::SegmentDriver(int pinDIN, int pinCLK, int pinCS, int ADDRESS) {
  MOSI = pinDIN;
  CLK = pinCLK;
  CS = pinCS;
  address = ADDRESS;

  pinMode(MOSI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);

  digitalWrite(CS, HIGH);

  // Disable display test
  if (sendSPI(0, CTRL_DISPLAYTEST) != 0) {
    Serial.println("Test display failed.");
  }

  // Set scan limit to max use (7)
  if (setScanLimit(7) != 0) {
    Serial.println("Setting scan limit failed.");
  }

  // Set brightness low value
  if (setBrightness(15) != 0) {
    Serial.println("Setting brightness failed.");
  }

  clear();

  turnOn();


  clear();
  delay(1000);
}

int SegmentDriver::turnOn() {
  return sendSPI(1, CTRL_SHUTDOWN);
}

int SegmentDriver::turnOff() {
  return sendSPI(0, CTRL_SHUTDOWN);
}

void SegmentDriver::clear() {
    setString("        ");
}

int SegmentDriver::setBrightness(int value) {
  if (value < 0 || value > 15) {
    return -1;
  }

  return sendSPI(value, CTRL_INTENSITY);
}

int SegmentDriver::setScanLimit(int limit) {
  if (limit < 0 || limit > 7) {
    return -1;
  }

  return sendSPI(limit, CTRL_SCANLIMIT);
}

int SegmentDriver::setChar(int place, char chr) {

  byte val = (byte)chr;

  // Convert to upper case, if the char is lower case.  
  if (val >= 97 && val <= 122) {
    val -= 32;
  }

  val -= ASCII_OFFSET;

  if (val < 0 || val > 95) {
    val = 2;
  }
  
  return sendSPI(pgm_read_byte_near(charTable + val), place + 1);
}

int SegmentDriver::setString(String string) {
  
  for (int i = 0; i < sizeof(string) - 1; i++) {
    setChar((DISPLAY_LENGTH - 1) - i, string[i]);
  }
  return 0;
}

int SegmentDriver::sendSPI(volatile byte data, volatile byte instruction) {
  digitalWrite(CS, LOW);
  shiftOut(MOSI, CLK, MSBFIRST, instruction);
  shiftOut(MOSI, CLK, MSBFIRST, data);
  digitalWrite(CS, HIGH);

  return 0;
}