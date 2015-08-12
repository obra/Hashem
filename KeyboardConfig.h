//#define DEBUG_SERIAL false
#ifndef KEYBOARD_CONFIG_H
#define KEYBOARD_CONFIG_H

#define EEPROM_KEYMAP_LOCATION 0

#define KEYS 24
static const byte pins[KEYS] = { A5,A4,A3,A2,A1,A0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,MISO,MOSI,SCK,SS};
static const byte keymap[KEYS] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X'};


#endif
