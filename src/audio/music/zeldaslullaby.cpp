#include <Arduino.h>
#include "audio\scale.h"

void play_zeldaslullaby(int buzzer)
{
    // change this to make the song slower or faster
    const int tempo_zeldaslullaby = 108;

    const int melody_zeldaslullaby[] = {
        // Zelda's Lullaby - The Legend of Zelda Ocarina of Time. 
  // Score available at https://musescore.com/user/12754451/scores/2762776
  
  NOTE_E4,2, NOTE_G4,4,
  NOTE_D4,2, NOTE_C4,8, NOTE_D4,8, 
  NOTE_E4,2, NOTE_G4,4,
  NOTE_D4,-2,
  NOTE_E4,2, NOTE_G4,4,
  NOTE_D5,2, NOTE_C5,4,
  NOTE_G4,2, NOTE_F4,8, NOTE_E4,8, 
  NOTE_D4,-2,
  NOTE_E4,2, NOTE_G4,4,
  NOTE_D4,2, NOTE_C4,8, NOTE_D4,8, 
  NOTE_E4,2, NOTE_G4,4,
  NOTE_D4,-2,
  NOTE_E4,2, NOTE_G4,4,

  NOTE_D5,2, NOTE_C5,4,
  NOTE_G4,2, NOTE_F4,8, NOTE_E4,8, 
  NOTE_F4,8, NOTE_E4,8, NOTE_C4,2,
  NOTE_F4,2, NOTE_E4,8, NOTE_D4,8, 
  NOTE_E4,8, NOTE_D4,8, NOTE_A3,2,
  NOTE_G4,2, NOTE_F4,8, NOTE_E4,8, 
  NOTE_F4,8, NOTE_E4,8, NOTE_C4,4, NOTE_F4,4,
  NOTE_C5,-2};

    // sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
    // there are two values per note (pitch and duration), so for each note there are four bytes
    const int notes = sizeof(melody_zeldaslullaby) / sizeof(melody_zeldaslullaby[0]) / 2;

    // this calculates the duration of a whole note in ms
    const int wholenote = (60000 * 4) / tempo_zeldaslullaby;

    int divider = 0, noteDuration = 0;
#if defined(ESP32)
    const unsigned int soundChannel = 0;
    ledcAttachPin(buzzer, soundChannel);
#endif

    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
    {

        // calculates the duration of each note
        divider = melody_zeldaslullaby[thisNote + 1];
        if (divider > 0)
        {
            // regular note, just proceed
            noteDuration = (wholenote) / divider;
        }
        else if (divider < 0)
        {
            // dotted notes are represented with negative durations!!
            noteDuration = (wholenote) / abs(divider);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
#if defined(ESP32)
        ledcWriteTone(soundChannel, melody_zeldaslullaby[thisNote]);
#else
        tone(buzzer, melody_zeldaslullaby[thisNote], noteDuration * 0.9);
#endif

        // Wait for the specief duration before playing the next note.
        delay(noteDuration);

        // stop the waveform generation before the next note.
#if defined(ESP32)
        ledcWriteTone(soundChannel, 0);
#else
        noTone(buzzer);
#endif
    }
#if defined(ESP32)
    ledcDetachPin(buzzer);
#endif
}