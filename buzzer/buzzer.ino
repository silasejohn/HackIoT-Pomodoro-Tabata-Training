#include "pitches.h"
// notes in the melody:
//int melody[] = {
//NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
//int melody[] = {NOTE_FS5, NOTE_A5, NOTE_CS6, 0, NOTE_A5, 0, NOTE_FS5, NOTE_D5, NOTE_D5, NOTE_D5, 0, NOTE_CS5, NOTE_D5, NOTE_FS5, NOTE_A5, NOTE_CS6, 0, NOTE_A5, 0, NOTE_FS5, NOTE_E6, NOTE_DS6, NOTE_D6};
int melody[] = 
{NOTE_E6, NOTE_DS6, 
NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B5, NOTE_D6, NOTE_C6, 
NOTE_A5, 0, 0, NOTE_C5, NOTE_E5, NOTE_A5,
NOTE_B5, 0, 0, NOTE_E5, NOTE_GS5, NOTE_B5,
NOTE_C6, 0, 0, NOTE_E5, NOTE_E6, NOTE_DS6,
NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B5, NOTE_D6, NOTE_C6, 
NOTE_A5, 0, 0, NOTE_C5, NOTE_E5, NOTE_A5,
NOTE_B5, 0, 0, NOTE_D5, NOTE_C6, NOTE_B5,
NOTE_A5
};
//int dur[] = {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 2};
int duration = 100; // 500 milliseconds
size_t size = sizeof(melody)/sizeof(melody[0]);
void setup() {
}
void loop() {
  
  for (int thisNote = 0; thisNote < size; thisNote++) {
  // pin8 output the voice, every scale is 0.5 sencond
  tone(11, melody[thisNote], 200);
  // Output the voice after several minutes
  //delay(dur[thisNote]*200);
  delay(250);
  }
  // restart after two seconds
  delay (400);
}
