// Copyright 2015 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details


#include "Hashem.h"
#include "digitalWriteFast.h"


#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);


void setup()
{
  wdt_disable();
  Keyboard.begin();
  
   reset_key_report();
  //blank out the matrix.
  for (byte key = 0; key < KEYS; key++) {
    physicalState[key] = 0;
  }

    for (byte key = 0; key < KEYS; key++) {
    make_input(pins[key]);
  }
}

String myApp;

void loop()
{
  //scan the Keyboard looking for connections
  for (byte key = 0; key < KEYS; key++) {
    physicalState[key] <<= 1;

    if (digitalRead(pins[key])) {
      physicalState[key] |= 0;
    } else {
      physicalState[key] |= 1;
    }

    send_key_event(key);


  }
  release_keys_not_being_pressed();
 Keyboard.sendCurrentReport();
  reset_key_report();
}


void release_keys_not_being_pressed()
{
  // we use charsReportedLastTime to figure out what we might
  // not be holding anymore and can now release. this is
  // destructive to charsReportedLastTime

  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // for each key we were holding as of the end of the last cycle
    // see if we're still holding it
    // if we're not, call an explicit Release

    if (charsReportedLastTime[i] != 0x00) {
      // if there _was_ a character in this slot, go check the
      // currently held characters
      for (byte j = 0; j < KEYS_HELD_BUFFER; j++) {
        if (charsReportedLastTime[i] == charsBeingReported[j]) {
          // if's still held, we don't need to do anything.
          charsReportedLastTime[i] = 0x00;
          break;
        }
      }
      Keyboard.release(charsReportedLastTime[i]);

    }
  }
}

void record_key_being_pressed(byte character)
{
  for (byte i = 0; i < KEYS_HELD_BUFFER; i++) {
    // todo - deal with overflowing the n key buffer here
    if (charsBeingReported[i] == 0x00) {
      charsBeingReported[i] = character;
      break;
    }
  }
}

void reset_key_report()
{
  memcpy( charsReportedLastTime, charsBeingReported, KEYS_HELD_BUFFER);
  memset(charsBeingReported, 0, KEYS_HELD_BUFFER);

}


void send_key_event(byte key)
{
  byte switchState = physicalState[key];
  byte mappedKey = keymap[key];

  if (key_is_pressed(switchState)) {
    record_key_being_pressed(mappedKey);
    if (key_toggled_on (switchState)) {
      Keyboard.press(mappedKey);
    }
  } else if (key_toggled_off (switchState)) {
    Keyboard.release(mappedKey);
  }

}


void make_input( int pin) {
  pinMode(pin, INPUT);  // Set pin as an input
  digitalWrite(pin, HIGH);  // Activate pull-up


}

