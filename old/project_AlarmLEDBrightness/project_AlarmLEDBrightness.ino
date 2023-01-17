#include "themes.h"
#include <TimeLib.h>

int alarmTimeHuman[5] = {2023,1,13,13,0}; // get alarm time from server (year, month, day, hour, minute)
int nowTimeHuman[5] = {2023,1,13,12,55}; // get current now from server (year, month, day, hour, minute)
int chosenTheme = 4; // get chosen theme from server 
long alarmTimeEpoch, nowTimeEpoch; // declare epoch time vars 
int timeBeforeAlarm = 15; // get time (in minutes) before alarm to start LED's from server


// Pins
const int led = 3;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);

  alarmTimeEpoch = toEpochTime(alarmTimeHuman[0],alarmTimeHuman[1],alarmTimeHuman[2],alarmTimeHuman[3],alarmTimeHuman[4]);
  nowTimeEpoch = toEpochTime(nowTimeHuman[0],nowTimeHuman[1],nowTimeHuman[2],nowTimeHuman[3],nowTimeHuman[4]);
  nowTimeEpoch = 0;

  Serial.println(chosenTheme);
  //Play_Pirates();
}

void loop() {
  // put your main code here, to run repeatedly:

  if((alarmTimeEpoch - nowTimeEpoch) <= timeBeforeAlarm*60){ // 15 minutes before the alarm, start turning on the LED
    float ledBrightness = 255 - 255 / (1 + exp(- ((alarmTimeEpoch - nowTimeEpoch)/(timeBeforeAlarm*5) - 5))); // brightness as sigmoid function, =0 before alarm, =255 after alarm.
    analogWrite(led,floor(ledBrightness));
    Serial.println(ledBrightness);
  }
  if(100 == nowTimeEpoch){
    if (chosenTheme == 1){
      Play_Pirates();
    }
    else if(chosenTheme == 2){
      Play_CrazyFrog();
    }
    else if(chosenTheme == 3){
      Play_MarioUW();
    }
    else if(chosenTheme == 4){
      Play_PinkPanther();
    }

  }
  nowTimeEpoch = nowTimeEpoch + 10; 
  Serial.println(nowTimeEpoch);
  delay(300);// simulate time running

}

long toEpochTime(int currentYear, int currentMonth,int currentMonthDay, int currentHour, int currentMinute){
  /* Description
  Function that converts date/time format in year/month/day/hour/minute to epoch time in seconds since 1970/01/01. 
  Input[int, int, int, int, int]: Current time in year, month, day, hour and minute.
  Output [long]: Current time in epoch time.
  */
  setTime(currentHour,currentMinute, 0, currentMonthDay, currentMonth, currentYear); // hour,min,sec,day,month,year
  return now();
}