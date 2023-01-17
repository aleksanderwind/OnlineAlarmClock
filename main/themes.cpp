#include "themes.h"

int buzzer_pin;

bool itr = false; //interrupt

/*
 * The themes given below were converted from the piano sheet music 
 * Code by : B.Aswinth Raj
 * Dated: 15-06-2017
 * Website: CircuitDigest.com
 */

//##############**"HE IS A PIRATE" Theme song of Pirates of caribbean**##############//
int Pirates_note[] = {
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, 
NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_D4,  
NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, 
NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
0, NOTE_A3, NOTE_C4, NOTE_B3, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_F4,
NOTE_F4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4,
NOTE_D4, 0, 0, NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4,
NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_G4,
NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_E3, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, 
NOTE_D4, 0, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4
};
int Pirates_duration[] = {
4,8,4,8,4,8,8,8,8,4,8,4,8,4,8,8,8,8,4,8,4,8,
4,8,8,8,8,4,4,8,8,4,4,8,8,4,4,8,8,
8,4,8,8,8,4,4,8,8,4,4,8,8,4,4,8,4,
4,8,8,8,8,4,4,8,8,4,4,8,8,4,4,8,8,
8,4,8,8,8,4,4,4,8,4,8,8,8,4,4,8,8
};
//###########End of He is a Pirate song#############//


//##############**"Crazy Frog" song of Crazy frog album**##############//
int CrazyFrog_note[] = {
  NOTE_D4, 0, NOTE_F4, NOTE_D4, 0, NOTE_D4, NOTE_G4, NOTE_D4, NOTE_C4,
  NOTE_D4, 0, NOTE_A4, NOTE_D4, 0, NOTE_D4, NOTE_AS4, NOTE_A4, NOTE_F4,
  NOTE_D4, NOTE_A4, NOTE_D5, NOTE_D4, NOTE_C4, 0, NOTE_C4, NOTE_A3, NOTE_E4, NOTE_D4,
  0,NOTE_D4,NOTE_D4
};
int CrazyFrog_duration[] = {
  8, 8, 6, 16, 16, 16, 8, 8, 8, 
  8, 8, 6, 16, 16, 16, 8, 8, 8,
  8, 8, 8, 16, 16, 16, 16, 8, 8, 2,
  8,4,4
};
//###########End of Crazy Frog#############//



//##############**"Mario underworld" **##############//
int MarioUW_note[] = {
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4,NOTE_AS3, NOTE_AS4, 0, 0,
  NOTE_C4, NOTE_C5, NOTE_A3, NOTE_A4, NOTE_AS3, NOTE_AS4, 0,0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,NOTE_DS3, NOTE_DS4, 0, 0,
  NOTE_F3, NOTE_F4, NOTE_D3, NOTE_D4,NOTE_DS3, NOTE_DS4, 0,
  0, NOTE_DS4, NOTE_CS4, NOTE_D4,
  NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3, NOTE_G3, NOTE_CS4,
  NOTE_C4, NOTE_FS4, NOTE_F4, NOTE_E3, NOTE_AS4, NOTE_A4,
  NOTE_GS4, NOTE_DS4, NOTE_B3,  NOTE_AS3, NOTE_A3, NOTE_GS3,0, 0, 0
};
int MarioUW_duration[] = {
 12, 12, 12, 12,12, 12, 6,3,
 12, 12, 12, 12, 12, 12, 6, 3,
 12, 12, 12, 12, 12, 12, 6,
 3, 12, 12, 12, 12,
 12, 12, 6, 6, 18, 18, 18,
 6, 6, 6, 6,6, 6,
 18, 18, 18, 18, 18, 18, 10, 10, 10,
 10, 10, 10, 3, 3, 3
};
//###########End of Mario underworld#############//


//##############**"Titanic" **##############//
int Titanic_note[] = {
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_F5,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_B4, NOTE_E4,
NOTE_E4, NOTE_B4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_E5, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_B3, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_B3, NOTE_E4,
NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4


};
int Titanic_duration[] = {
8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,4,4,8,8,8,8,8,8,4,8,8,8,8,8,8,4,8,8,8,8,8,8,1,
8,8,8,8,4,4,4,8,4,4,8,8,8,8,4,8,8,4,8,4,8,8,4,8,4,1



};
//###########End of Titanic#############//


//##############**"Pink Panther" **##############//

int PinkPanther_note[] = {
  0, 0, 0, NOTE_DS4, 
  NOTE_E4, 0, NOTE_FS4, NOTE_G4, 0, NOTE_DS4,
  NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, 0, 0, NOTE_DS4,
  
  NOTE_E4, 0, NOTE_FS4, NOTE_G4, 0, NOTE_DS4,
  NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_G4, NOTE_B4, NOTE_E5,
  NOTE_DS5,   
  NOTE_D5, 0, 0, NOTE_DS4, 
  NOTE_E4, 0, NOTE_FS4, NOTE_G4, 0, NOTE_DS4,
  NOTE_E4, NOTE_FS4,  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_E4, NOTE_G4, NOTE_B4,   
  
  NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_E4, NOTE_D4, 
  NOTE_E4, 0,
  0, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_E4,
  NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4, NOTE_AS4, NOTE_A4,   
  NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_E4, NOTE_E4
};

int PinkPanther_duration[] = {
  2, 4, 8, 8, 
  4, 8, 8, 4, 8, 8,
  8, 8,  8, 8, 8, 8, 8, 8,   
  2, 16, 16, 16, 16, 
  2, 4, 8, 4,
  
  4, 8, 8, 4, 8, 8,
  8, 8,  8, 8, 8, 8, 8, 8,
  1,   
  2, 4, 8, 8, 
  4, 8, 8, 4, 8, 8,
  8, 8,  8, 8, 8, 8, 8, 8,   
  
  2, 16, 16, 16, 16, 
  4, 4,
  4, 8, 8, 8, 8, 8, 8,
  16, 8, 16, 8, 16, 8, 16, 8,   
  16, 16, 16, 16, 16, 2
};

//###########End of Pink Panther#############//

void initBuzzer(int buzz_pin){
  buzzer_pin = buzz_pin;
}

void Play_Pirates()
{ 
  
  itr = false;

  for (int thisNote = 0; thisNote < (sizeof(Pirates_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / Pirates_duration[thisNote];//convert duration to time delay

    tone(buzzer_pin, Pirates_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.05; //Here 1.05 is tempo, increase to play it slower

    delay(pauseBetweenNotes);

    noTone(buzzer_pin); //stop music on pin 2 
    
    if(itr){break;}

    }

}

void Play_CrazyFrog()
{

  itr = false;
  
  for (int thisNote = 0; thisNote < (sizeof(CrazyFrog_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / CrazyFrog_duration[thisNote]; //convert duration to time delay

    tone(buzzer_pin, CrazyFrog_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.9;//Here 1.30 is tempo, decrease to play it faster

    delay(pauseBetweenNotes);

    noTone(buzzer_pin); //stop music on pin 2 

    if(itr){break;}
    
    }

}

void Play_MarioUW()
{
    itr = false;
  
    for (int thisNote = 0; thisNote < (sizeof(MarioUW_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / MarioUW_duration[thisNote];//convert duration to time delay

    tone(buzzer_pin, MarioUW_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 1.80;

    delay(pauseBetweenNotes);

    noTone(buzzer_pin); //stop music on pin 2 
      
    if(itr){break;}

    }

}

void Play_Titanic()
{
    itr = false;
  
    for (int thisNote = 0; thisNote < (sizeof(Titanic_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / Titanic_duration[thisNote];//convert duration to time delay

    tone(buzzer_pin, Titanic_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 2.70;

    delay(pauseBetweenNotes);

    noTone(buzzer_pin); //stop music on pin 2 

    if(itr){break;}
      
    }

}

void interrupt()
{
  //Serial.println("I interrupted, SORRY!"); DEBUG
  itr = true;
}

void Play_PinkPanther()
{
    itr = false;

    for (int thisNote = 0; thisNote < (sizeof(PinkPanther_note)/sizeof(int)); thisNote++) {


    int noteDuration = 1000 / PinkPanther_duration[thisNote];//convert duration to time delay

    tone(buzzer_pin, PinkPanther_note[thisNote], noteDuration);


    int pauseBetweenNotes = noteDuration * 2.10;

    delay(pauseBetweenNotes);

    noTone(buzzer_pin); //stop music on pin 2 

    if(itr){break;}
      
    }

}
