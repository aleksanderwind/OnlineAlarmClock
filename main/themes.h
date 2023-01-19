#ifndef themes_h
#define themes_h

/*
Header file created by: JKT
Authors for individual functions can be found in the themes.cpp file.
*/

#include "pitches.h"
#include "Arduino.h"

extern bool itr; //interrupt

/*
 * The themes given below were converted from the piano sheet music 
 * Code by : B.Aswinth Raj
 * Dated: 15-06-2017
 * Website: CircuitDigest.com
 */

void initBuzzer(int buzz_pin);

void Play_Pirates();
void Play_CrazyFrog();
void Play_MarioUW();
void Play_PinkPanther();
void interrupt();

#endif // themes_h
