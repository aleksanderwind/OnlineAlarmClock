#include "pitches.h"
#include "themes.h"
#include <TimeLib.h>

int alarmTimeHuman[5] = {2023,1,13,13,0}; // get alarm time from server (year, month, day, hour, minute)
int nowTimeHuman[5] = {2023,1,13,12,55}; // get current now from server (year, month, day, hour, minute)
int chosenTheme = 2; // get chosen theme from server 
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

  Serial.println(chosenTheme);
}

void loop() {
  // put your main code here, to run repeatedly:

  if((alarmTimeEpoch - nowTimeEpoch) <= timeBeforeAlarm*60){ // 15 minutes before the alarm, start turning on the LED
    float ledBrightness = 255 - 255 / (1 + exp(- ((alarmTimeEpoch - nowTimeEpoch)/(timeBeforeAlarm*5) - 5))); // brightness as sigmoid function, =0 before alarm, =255 after alarm.
    analogWrite(led,floor(ledBrightness));
    Serial.println(ledBrightness);
  }
  if(alarmTimeEpoch == nowTimeEpoch){
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

// Theme functions. Should perhaps be included in separate .cpp file, but haven't figured out how yet. (Compilation error: 'NOTE_D4' was not declared in this scope).

void Play_Pirates()
{ 

  for (int thisNote = 0; thisNote < (sizeof(Pirates_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay

    tone(2, Pirates_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower

    delay(pauseBetweenNotes);

    noTone(2); //stop music on pin 2 

    }

}

void Play_CrazyFrog()
{

  for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / CrazyFrog_duration[thisNote]; //convert duration to time delay

    tone(2, CrazyFrog_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.30;//Here 1.30 is tempo, decrease to play it faster

    delay(pauseBetweenNotes);

    noTone(2); //stop music on pin 2 

    }

}

void Play_MarioUW()
{

    for (int thisNote = 0; thisNote < (sizeof(MarioUW_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / MarioUW_duration[thisNote];//convert duration to time delay

    tone(2, MarioUW_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.80;

    delay(pauseBetweenNotes);

    noTone(2); //stop music on pin 2 

    }

}

void Play_Titanic()
{

    for (int thisNote = 0; thisNote < (sizeof(Titanic_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / Titanic_duration[thisNote];//convert duration to time delay

    tone(2, Titanic_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 2.70;

    delay(pauseBetweenNotes);

    noTone(2); //stop music on pin 2 

    }

}

void Play_PinkPanther()
{

    for (int thisNote = 0; thisNote < (sizeof(PinkPanther_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / PinkPanther_duration[thisNote];//convert duration to time delay

    tone(2, PinkPanther_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 2.10;

    delay(pauseBetweenNotes);

    noTone(2); //stop music on pin 2 

    }

}
