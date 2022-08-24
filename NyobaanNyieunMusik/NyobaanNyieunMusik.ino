/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8

  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"

#define notes 8

// notes in the melody:
int melody[][notes] = {
//  {F4, F4, F4, F4, E4, F4, F4, E4, F4, G4, A4, G4},
  {F4, G4, A4},
  {}
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[][notes] = {
//  {2, 4, 2, 2, 2, 1, 2, 2, 1, 2, 1, 1},
  {4,4,1},
  {}
};

void setup() {
  // iterate over the notes of the melody:

  Serial.begin(115200);
}

void loop() {
  // no need to repeat the melody.
  int arrSize = sizeof(melody) / sizeof(melody[0]);

  Serial.println(arrSize);

  for (int line = 0; line < arrSize; line++) {
    for (int thisNote = 0; thisNote < notes; thisNote++) {

      // to calculate the note duration, take one second divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / noteDurations[line][thisNote];
      tone(8, melody[line][thisNote], noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(8);
    }
  }
}
