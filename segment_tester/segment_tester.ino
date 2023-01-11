#include "seven_segment_MAX7219.h"

#define DIN 13  //D7 on ESP8266
#define CLK 14  //D5 on ESP8266
#define CS 15   //D8 on ESP8266

SegmentDriver display = SegmentDriver(DIN, CLK, CS, 0);
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  display.clear();
  delay(500);
  display.setString("01234567");
  delay(1000);
  display.clear();
  while (true) { delay(100); }
}
