//#define DEBUG_SERIAL false
#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

#define EEPROM_KEYMAP_LOCATION 0

#define KEYS 24

/* Arduino Micro pins, scanning from left to right, top to down */
static const byte pins[KEYS] = {
     12, 11, 10, 9, 8,    7, 6, 5, 4, 3,
    2, 0, 1, SS, MOSI,    13, A0, A1, A2, A3,
               A4, A5,    MISO, SCK
};

/* Plover-compatible 24 key layout, scanning from left to right, top to down*/
static const byte keymap[KEYS] = {
  'q','w','e','r','t',    'u','i','o','p','[',
  'a','s','d','f','g',    'j','k','l',';','\'',
              'c','v',    'n','m'
};

#endif
