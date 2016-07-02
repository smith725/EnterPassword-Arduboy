/*
EnterPassword
June 29, 2016
Copyright (C) 2016 Chris Smith
All rights reserved.

Allows editting of a single character string

Several basic operations relating to creating sending storing loading password
*/

#include "Arduboy.h"
#include "Keyboard.h"
#include "EEPROM.h"

Arduboy arduboy;

// globals needed every pass through loop

const char    valid[]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz01234567890123456789!#$%&()*+,-.:;=?@[]^_{}!#$%&()*+,-.:;=?@[]^_{}";
const uint8_t PW_ALL   = 118;
const uint8_t PW_AN    = 72; 
const uint8_t PW_ALPHA = 52; 

char    password[22];
uint8_t cursor;
uint8_t lastinput;
uint8_t cmdmode;

struct eeprom_password {
  uint32_t magic;
  char     password[24];
};

#define EEPROM_PASSWORD (123123123)

// runs once
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // set the framerate to 15
  arduboy.setFrameRate(15);

  // last button press was nothing
  lastinput = 0;
  cmdmode = 0;

  // initialize the word
  deletepassword();
    
  // first we clear our screen to black
  arduboy.clear();
}

void sendpassword() {
  Keyboard.begin();
  Keyboard.print(password);
  Keyboard.end();
}

void randompassword() {
  uint8_t i;
  randomSeed( micros() );
  password[0] = valid[random(PW_ALPHA)];
  for (i=1; i<21; i++) {
    password[i] = valid[random(PW_ALL)];
  }
cursor=0;
}

void randomAMpassword() {
  uint8_t i;
  randomSeed( micros() );
  password[0] = valid[random(PW_ALPHA)];
  for (i=1; i<21; i++) {
    password[i] = valid[random(PW_AN)];
  }
cursor=0;
}

void deletepassword() {
  uint8_t i;
  for (i=0; i<21; i++) {
    password[i] = ' ';
  }
password[21] = 0;
cursor = 0;
}

void putpassword() {
  eeprom_password eppw = { EEPROM_PASSWORD, "" };

  strcpy( eppw.password, password );

  EEPROM.put( EEPROM_STORAGE_SPACE_START, eppw );
}

void getpassword() {
  eeprom_password eppw = { (0), "" };

  EEPROM.get( EEPROM_STORAGE_SPACE_START, eppw );

  if ( eppw.magic == EEPROM_PASSWORD ) {
    strcpy( password, eppw.password );
    cursor = 0;
  }
}



// main loop
void loop() {

  // loop variables

  uint8_t buttons;

  // wait for the next frame time
  if (!(arduboy.nextFrame()))
    return;

  // fetch all buttons up/down
  buttons = arduboy.buttonsState();
  
  if (!lastinput) {
    // filter buttons if cmdmode
    if (cmdmode) {
      buttons = buttons & (A_BUTTON + B_BUTTON);
    }
    
    if(buttons == LEFT_BUTTON) {
      if (cursor>0) cursor--;
      }
    if(buttons == RIGHT_BUTTON) {
      if (cursor<20) cursor++;
      }
    if(buttons == UP_BUTTON) {
      password[cursor]++;
      if (password[cursor]>(char)126) password[cursor] = (char)33;  
    }
    if(buttons == DOWN_BUTTON) {
      password[cursor]--;
      if (password[cursor]<(char)33) password[cursor] = (char)126;  
    }

    if(buttons == A_BUTTON) {
      cmdmode = (cmdmode+1) % 8 ;
    }

    if(cmdmode > 0 && buttons == B_BUTTON) {
      switch (cmdmode) {
        case 1: sendpassword();
                break;
        case 2: randompassword();
                sendpassword();
                break;
        case 3: randompassword();
                break;
        case 4: randomAMpassword();
                break;
        case 5: deletepassword();
                break;
        case 6: putpassword();
                break;
        case 7: getpassword();
                break;
      }
      cmdmode = 0; 
    }

  }

  // delete to LEFT
  if(buttons == LEFT_BUTTON + B_BUTTON && lastinput == B_BUTTON) {
    uint8_t i;
    if (cursor>0) {
      for (i=cursor-1; i<21; i++) {
        password[i]=password[i+1];
      }
      password[21]=' ';
      cursor--;
    }
  }
  
  // insert space
  if(buttons == RIGHT_BUTTON + B_BUTTON && lastinput == B_BUTTON) {
    uint8_t i;
    for (i=21; i>cursor; i--) {
      password[i]=password[i-1];
    }
    password[cursor]=' ';
  }

  if(buttons == UP_BUTTON + B_BUTTON) {
    password[cursor]++;
    if (password[cursor]>(char)126) password[cursor] = (char)33;  
  }

  if(buttons == DOWN_BUTTON + B_BUTTON) {
    password[cursor]--;
    if (password[cursor]<(char)33) password[cursor] = (char)126;  
  }

  arduboy.clear();

  arduboy.setCursor(0,0);
  arduboy.print(F("Password Build + Send"));
  
  arduboy.setCursor(0,16);
  arduboy.print((password));
  arduboy.setCursor(6*cursor,8);
  arduboy.print(F("\x1E"));
  arduboy.setCursor(6*cursor,24);
  arduboy.print(F("\x1F"));

  if (cmdmode) {
    arduboy.setCursor(0,40);
    switch (cmdmode) {
//                            "012345678901234567890"
      case 1: arduboy.print(F("Send?")); break;
      case 2: arduboy.print(F("Random+Send?")); break;
      case 3: arduboy.print(F("Random?")); break;
      case 4: arduboy.print(F("Random alphanum?")); break;
      case 5: arduboy.print(F("Delete?")); break;
      case 6: arduboy.print(F("Save?")); break;
      case 7: arduboy.print(F("Load?")); break;
    }
    arduboy.setCursor(0,48);
    switch (cmdmode) {
//                            "012345678901234567890"
      case 1: arduboy.print(F("Send pw via keyboard")); break;
      case 2: arduboy.print(F("Random pw then send")); break;
      case 3: arduboy.print(F("Random pw, full chars")); break;
      case 4: arduboy.print(F("Random pw al,num only")); break;
      case 5: arduboy.print(F("Delete password")); break;
      case 6: arduboy.print(F("Store password EEPROM")); break;
      case 7: arduboy.print(F("Fetch password EEPROM")); break;
    }
    arduboy.setCursor(0,56);
//                  "012345678901234567890"
    arduboy.print(F("A next cmd, B do this"));
  }
  else {
//                                           "012345678901234567890"
    arduboy.setCursor(0,40); arduboy.print(F("L,R move, U,D change"));
    arduboy.setCursor(0,48); arduboy.print(F("B+ L/del R/ins UDfast"));
    arduboy.setCursor(0,56); arduboy.print(F("A - commands"));
  }
  
  // show all that screen effort
  arduboy.display();

  // remember what was pressed last time
  lastinput = buttons;
}
