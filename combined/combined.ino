#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#include "pitches.h"
// notes in the melody:
//int melody[] = {
//NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int melody[] = {NOTE_FS5, NOTE_A5, NOTE_CS6, 0, NOTE_A5, 0, NOTE_FS5, NOTE_D5, NOTE_D5, NOTE_D5, 0, NOTE_CS5, NOTE_D5, NOTE_FS5, NOTE_A5, NOTE_CS6, 0, NOTE_A5, 0, NOTE_FS5, NOTE_E6, NOTE_DS6, NOTE_D6};
/*int melody[] =
  {NOTE_E6, NOTE_DS6,
  NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B5, NOTE_D6, NOTE_C6,
  NOTE_A5, 0, 0, NOTE_C5, NOTE_E5, NOTE_A5,
  NOTE_B5, 0, 0, NOTE_E5, NOTE_GS5, NOTE_B5,
  NOTE_C6, 0, 0, NOTE_E5, NOTE_E6, NOTE_DS6,
  NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_B5, NOTE_D6, NOTE_C6,
  NOTE_A5, 0, 0, NOTE_C5, NOTE_E5, NOTE_A5,
  NOTE_B5, 0, 0, NOTE_D5, NOTE_C6, NOTE_B5,
  NOTE_A5
  };*/
int dur[] = {2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 2};
int duration = 100; // 500 milliseconds
size_t size = sizeof(melody) / sizeof(melody[0]);

void setup ()
{
  Serial.begin(9600);
  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");

    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    // rtc.adjust(DateTime(2017, 1, 27, 12, 56, 0));
  }
}

void loop () {
    DateTime now = rtc.now();

    // calculate a date which is 7 days & 30 seconds into the future
    DateTime future (now - TimeSpan(1,-9,10,30));
    
    //Serial.println("Future Date & Time (Now + 7days & 30s): ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    if (future.second() % 15 == 0){
      for (int thisNote = 0; thisNote < size; thisNote++) {
        // pin8 output the voice, every scale is 0.5 sencond
        tone(11, melody[thisNote], dur[thisNote] * 100);
        // Output the voice after several minutes
        delay(dur[thisNote]*200);
        //delay(250);
      }
    }
    
    Serial.println();
/*
  for (int thisNote = 0; thisNote < size; thisNote++) {

    DateTime now = rtc.now();

    Serial.println("Current Date & Time: ");

    // pin8 output the voice, every scale is 0.5 sencond
    tone(11, melody[thisNote], 200);
    // Output the voice after several minutes
    delay(dur[thisNote] * 200);
    //delay(250);
    DateTime future (now - TimeSpan(1, -9, 10, 30));

    // Serial.println("Future Date & Time (Now + 7days & 30s): ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();

    Serial.println();

  }
  // restart after two seconds
  delay (400);*/
}
