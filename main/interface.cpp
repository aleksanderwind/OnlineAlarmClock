/*
  Interface including seven segment display, sensor readings and alarm management.
*/

#include "interface.h"

extern bool itr;

LED* led_Strip; // variable for importing led class

/* 
Class initializer 
Takes 4 inputs:
pinDIN : Corresponds to DIN on the 7-segment board. Must be a vlid SPI MOSI pin. 
pinCLK : Corresponds to CLK on the 7-segment board. Must be a valid SPI CLK pin.
pinCS  : Corresponds to CS on the 7-segment board. Can be any digital pin.

Author: JKT
*/
SegmentDriver::SegmentDriver(int pinDIN, int pinCLK, int pinCS) {
  MOSI = pinDIN;
  CLK = pinCLK;
  CS = pinCS;

  pinMode(MOSI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);

  digitalWrite(CS, HIGH); // Pull CS high to lock (latch) display

  // Disable display test
  if (sendSPI(0, CTRL_DISPLAYTEST) != 0) {
    Serial.println("Test display failed.");
  }

  // Set scan limit to max use (7)
  if (setScanLimit(7) != 0) {
    Serial.println("Setting scan limit failed.");
  }

  // Set brightness low value
  if (setBrightness(1) != 0) {
    Serial.println("Setting brightness failed.");
  }

  // Power on the display
  turnOn();

  // Clear the display
  clear();

  delay(1000);
}

/*
Turn on the display
Author: JKT
*/
int SegmentDriver::turnOn() {
  return sendSPI(1, CTRL_SHUTDOWN);
}

// Turn off the display
int SegmentDriver::turnOff() {
  return sendSPI(0, CTRL_SHUTDOWN);
}

// Turn on/off the display given threshold
void SegmentDriver::checkTimeout(long difference, long threshold){
  if (difference >= threshold) {
    turnOff();
  } else {
    turnOn();
  }
}

/* 
Clear the display, by settings all eight characters to space (" ")
Author: JKT
*/
void SegmentDriver::clear() {
  for (int i = 1; i < DISPLAY_LENGTH + 1; i++) {
    setString("        ");
  }
}

/*
Set the brightness of the display
Input: value (integer between 0-15)
Author: JKT
*/
int SegmentDriver::setBrightness(int value) {
  if (value < 0 || value > 15) {
    return -1;
  }

  return sendSPI(value, CTRL_INTENSITY);
}

/*
Set the scan limit of the display
Input: value (integer between 0-7)
Author: JKT
*/
int SegmentDriver::setScanLimit(int limit) {
  // If the given limit is not in the range of 0-7, return -1 indicating a failure.
  if (limit < 0 || limit > 7) {
    return -1;
  }

  return sendSPI(limit, CTRL_SCANLIMIT);
}

/*
Set a single character of the display
Inputs:
place: integer between 0-7
chr: any character. If the character cannot be printed (e.g. X, blank is printed)

returns 0 if OK. Anything other than 0 is assumed an error. 
Author: JKT
*/
int SegmentDriver::setChar(int place, char chr, bool dot = false) {

  byte val = (byte)chr;

  // Convert to upper case, if the char is lower case.
  // This corresponds to subtracting 32 from the decimal value of the given char.
  if (val >= 97 && val <= 122) {
    val -= 32;
  }

  // We further subtract the ASCII_OFFSET to achieve the right index into the asciiTableRef
  val -= ASCII_OFFSET;

  // If the final value is not an index in the asciiTableRef, we just use val = 2, 
  // which is equivivalent to a space (" ").
  if (val < 0 || val > sizeof(asciiTableRef)) {
    val = 2;
  }

  // Look into the asciiTableRef and get the corresponding byte.
  byte asciiRef = pgm_read_byte_near(asciiTableRef + val);

  // If the character should be printed with a dot (e.g 7.), 
  // add the dot to the byte. 
  if (dot) {
    asciiRef |= B10000000;
  }

  // Finally send the SPI command with the proper byte and and the right place
  return sendSPI(asciiRef, place + 1);
}


/*
Display a string on the 7-segment display
Inputs:
string: A string.

If the string is longer than 8 characters, only the first 8 characters are shown.

returns 0 if OK. Anything other than 0 is assumed an error. 
Author: JKT
*/
int SegmentDriver::setString(String string) {
  // Check if the string is longer than 8 characters.
  // sizeof(string) includes the end-of-line character '\0',
  // thus the length becomes one longer than the actual string. 
  // Subtracting 1 from the sizeof() corrects this. 
  int len = (sizeof(string) - 1 > DISPLAY_LENGTH) ? DISPLAY_LENGTH : sizeof(string) - 1;

  // Start sending characters, starting from the left.
  for (int i = 0; i < len; i++) {
    setChar((DISPLAY_LENGTH - 1) - i, string[i]);
  }
  return 0;
}

/*
Specific function to display the clock in the middle of the display,
with a dot seperating the numbers. 
The clock is in 24hr format.
Inputs:
interger hour, minute. 

Hour must be in the range [0-23] and minute must be [0-59].
Author: JKT
*/
int SegmentDriver::setClock(int hour, int minute){  
  if (hour < 0 || hour > 23) {
    return -1;
  }

  if (minute < 0 || hour > 59) {
    return -1;
  }

  // Check whether we should add a 0 before the hour number. 
  // If the hour is greater than or equal to 10, this is not necessary.
  String hr = (hour >= 10) ? String(hour) : "0" + String(hour);

  // Check whether we should add a 0 before the minute number. 
  // If the minute is greater than or equal to 10, this is not necessary.
  String min = (minute >= 10) ? String(minute) : "0" + String(minute);

  // Send the first number of hour
  setChar(5, hr[0]);
  // Send the second number of hour, with dot = true.
  // This will print the dot seperating hour and minutes.
  setChar(4, hr[1], true);

  // Send the first number of minute
  setChar(3, min[0]);
  // Send the final number of minute
  setChar(2, min[1]);

  return 0;
}

/*
SPI command for interfacing with the MAX7219 IC. 
Inputs:
data: a single byte of data (usually this is one charactar)
instruction: a single byte representing the control mode to be used. 
  (Refer to the Control Register mapping in the top of the file)

returns 0 if OK. Anything other than 0 is assumed an error. 
Author: JKT
*/
int SegmentDriver::sendSPI(volatile byte data, volatile byte instruction) {
  // Pull CS pin low to initiate communication
  digitalWrite(CS, LOW);

  // Send the instruction byte to the MAX7219
  shiftOut(MOSI, CLK, MSBFIRST, instruction);

  // Send the data byte to the MAX7219
  shiftOut(MOSI, CLK, MSBFIRST, data);

  // Pull CS high to latch the display
  digitalWrite(CS, HIGH);

  return 0;
}

/*
Parse LED object and store pointer
Author: AC
*/
void initLEDInInterface(LED* strip){
  led_Strip = strip;
}

/*

Author: AC
*/
void readSensors(data* sensorData, DHTsensor* sensor){
  sensorData->temperature = sensor->smoothTempDHT();
  delay(300);
  sensorData->humidity = sensor->smoothHumiDHT();
  sensorData->lightLevel = sensor->smoothLumen();
}

/*

Author: MAD
*/
void AlarmCheck(int timeBeforeAlarm, struct myTM* currentAlarm, struct myTM* currentTime, long colorValue, int currentSong, data* SensorData, float lumRef)
{  
  if((currentAlarm->inEpoch - currentTime->inEpoch) <= timeBeforeAlarm*60){ // X minutes before the alarm, start turning on the LED
  // brightness as sigmoid function, =0 before alarm, =1 after alarm.
  float ledBrightness = (1.0 / (1.0 + exp(((currentAlarm->inEpoch - currentTime->inEpoch)/(timeBeforeAlarm*5.0) - 5.0)))) - (SensorData->lightLevel/lumRef); 
  //float ledBrightness = 1.0 - (1.0 / (timeBeforeAlarm*60))*(currentAlarm->inEpoch - currentTime->inEpoch);
  led_Strip->setLEDStripHex(colorValue,ledBrightness);
  /* DEBUG
  Serial.println(ledBrightness);
  Serial.println(currentAlarm->inEpoch);
  Serial.println(currentTime->inEpoch);
  */
  }
  if(!itr){
    if((currentAlarm->inEpoch) <= (currentTime->inEpoch)){
    if (currentSong == 1){
      Play_Pirates();
    }
    else if(currentSong == 2){
      Play_CrazyFrog();
    }
    else if(currentSong == 3){
      Play_MarioUW();
    }
    else if(currentSong == 4){
      Play_PinkPanther();
    }

    }
  }
    
    
}
  
long toEpochTime(int currentYear, int currentMonth,int currentMonthDay, int currentHour, int currentMinute){
  /* Description
  Function that converts date/time format in year/month/day/hour/minute to epoch time in seconds since 1970/01/01. 
  Input[int, int, int, int, int]: Current time in year, month, day, hour and minute.
  Output [long]: Current time in epoch time.
  Author: MAD
  */
  setTime(currentHour,currentMinute, 0, currentMonthDay, currentMonth, currentYear); // hour,min,sec,day,month,year
  return now();
}
