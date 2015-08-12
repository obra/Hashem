// Copyright 2015 Keyboardio, inc. <jesse@keyboard.io>
// See "LICENSE" for license details


#include "Hashem.h"
#include "digitalWriteFast.h"
#include <Wire.h>


#define TS(X) //Serial.print(micros() );Serial.print("\t");Serial.println(X);


void setup_matrix()
{
  reset_key_report();
  //blank out the matrix.
  for (byte col = 0; col < COLS; col++) {
    for (byte row = 0; row < ROWS; row++) {
      matrixState[row][col] = 0;
    }
  }
}



void scan_matrix()
{
  //scan the Keyboard matrix looking for connections
  for (byte row = 0; row < LEFT_ROWS; row++) {
      matrixState[row][col] <<= 1;

      matrixState[row][(COLS - 1) - col] <<= 1;

      TS("Reading left pin")
      if (left_initted && leftsx1509.readPrefetchedPin(left_colpins[col])) {
        matrixState[row][col] |= 0;
      } else {
        matrixState[row][col] |= 1;
      }


      TS("calling send_key_event")
      send_key_event(row, col);

    }
    TS("clearing output pins")
      leftsx1509.updatePinState(left_rowpins[row], HIGH);
  }
  TS("Releasing keys not being pressed")
  release_keys_not_being_pressed();
  TS("Sending key report");
  Keyboard.sendCurrentReport();
  TS("clearing internal key report")
  reset_key_report();
}

void setup()
{
  wdt_disable();
  Keyboard.begin();
  setup_matrix();
  setup_pins();
}

String myApp;

void loop()
{
  TS("A noop takes...")
  TS("about to scan the matrix")
  scan_matrix();
  TS("updating LEDs");
  update_leds(temporary_keymap == NUMPAD_KEYMAP);
}





//

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
    // todo - deal with overflowing the 12 key buffer here
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


void send_key_event(byte row, byte col)
{
  byte switchState = matrixState[row][col];
  Key mappedKey = keymaps[temporary_keymap][row][col];

      if (key_is_pressed(switchState)) {
        record_key_being_pressed(mappedKey.rawKey);
        if (key_toggled_on (switchState)) {
          press_key(mappedKey);
        }
      } else if (key_toggled_off (switchState)) {
        release_key(mappedKey);
      }

}

void press_key(Key mappedKey) {
  Keyboard.press(mappedKey.rawKey);
}

void release_key(Key mappedKey) {
  Keyboard.release(mappedKey.rawKey);
}



void make_input( int pin) {
  sx1509.pinDir(pin, INPUT);  // Set SX1509 pin 1 as an input
  sx1509.writePin(pin, HIGH);  // Activate pull-up


}

void make_output(int pin) {
  sx1509.pinDir(pin, OUTPUT);
  sx1509.writePin(pin, HIGH);

}


void setup_pins() {
  left_initted = setup_sx1509(leftsx1509, left_colpins, left_rowpins);

}
