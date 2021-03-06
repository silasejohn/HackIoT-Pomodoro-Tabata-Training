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


/*#include <Keypad.h>
//four rows
const byte ROWS = 4;
//four columns
const byte COLS = 4;
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
{'1','2','3','A'},
{'4','5','6','B'},
{'7','8','9','C'},
{'*','0','#','D'}
};
//connect to the row pinouts of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6};
//connect to the column pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2};
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
String password = "ABC123";
String input = "";*/

// Pulse Sensor

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"



int hour = 0;
int minute = 0;
int second = 0;
bool check = false;

void alarm_init(){
  Serial.println("Type Hour");
  while (!check){
    while (Serial.available() == 0) { } 
    String hour_string = Serial.readStringUntil('\n');
    hour = hour_string.toInt();
    if (hour >= 0 && hour < 23){
      Serial.print("Hour set: ");
      Serial.println(hour);
      check = true;
    } else { 
      Serial.println("Invalid input");
    }
  }
  check = false;

  Serial.println("Type Minute");
  while (!check){
    while (Serial.available() == 0) { } 
    String min_string = Serial.readStringUntil('\n');
    minute = min_string.toInt();
    if (minute < 0 || minute > 59){
      Serial.println("Invalid input");
    } else { 
      Serial.print("Minute set: ");
      Serial.println(minute);
      check = true;
    }
  }
  check = false;

  Serial.println("Type Second");
  while (!check){
    while (Serial.available() == 0) { } 
    String sec_string = Serial.readStringUntil('\n');
    second = sec_string.toInt();
    if (second < 0 || second > 59){
      Serial.println("Invalid input");
    } else { 
      Serial.print("Second set: ");
      Serial.println(second);
      check = true;
    }
  }
}


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

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

  alarm_init();
}

int past_sec = 0;
bool alarming = false;
int bpm_count = 0;

void loop () {
    DateTime now = rtc.now();

    // calculate a date which is 7 days & 30 seconds into the future
    DateTime future (now - TimeSpan(1,-9,10,30));

    if (past_sec != future.second()){
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
      past_sec = future.second();
    }

    if (alarming && (future.second() + 60 - second) % 15 == 0){
      Serial.println();
      Serial.println("ALARM!!");
      Serial.println();
      for (int thisNote = 0; thisNote < size; thisNote++) {
        // pin8 output the voice, every scale is 0.5 sencond
        tone(11, melody[thisNote], dur[thisNote] * 100);
        // Output the voice after several minutes
        delay(dur[thisNote]*200);
        //delay(250);
      }
    }

    if (future.hour() == hour && future.minute() == minute && future.second() == second){
      alarming = true;
    }

    int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

    if (pulseSensor.sawStartOfBeat() && alarming) {            // Constantly test to see if "a beat happened". 
      Serial.println("???  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
      Serial.print("BPM: ");                        // Print phrase "BPM: " 
      Serial.println(myBPM);                        // Print the value inside of myBPM. 
      if (myBPM > 70){
        bpm_count++;
      }
      else {
        bpm_count = 0;
      }
    }

    if (bpm_count > 10){
      alarming = false;
    }

    delay(20);                    // considered best practice in a simple sketch.
/*
    char customKey = customKeypad.getKey();
  
    if (customKey){
      if (customKey == '*'){
        input = "";
      }
      else {
        input += customKey;
        Serial.println(input);
      }
    }
    
    if (input == password){
      Serial.println("Unlocked!");
    }*/

}
