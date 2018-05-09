/*
UPS Warning Light
Copyright (c) 2018, Matthew Tong
https://github.com/mtongnz/UPS_Warning_Light
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public
License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any
later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with this program.
If not, see http://www.gnu.org/licenses/
*/

#define FLASH_RATE 50

#define UPS_BATTERY 0
#define SW_WHITE 1
#define SW_BLUE 2
#define COL_RED 3
#define COL_BLUE 4
#define COL_WHITE 5

#define UPS_BAT_TIMEOUT 150  // How long (in ms) the UPS must have no power before we flash red (debounce)

enum states {
  OFF,
  WHITE,
  BLUE,
  RED,
  RED_WHITE,
  RED_BLUE
};

uint8_t curState = OFF, prevButton = 20;
bool isBattery = false;
uint16_t counter = 0;

long upsDebounce = 0;
long butDebounce = 0;
long debounceDelay = 50;

void setup() {
  pinMode(UPS_BATTERY, INPUT);
  pinMode(SW_WHITE, INPUT);
  pinMode(SW_BLUE, INPUT);
  pinMode(COL_RED, OUTPUT);
  pinMode(COL_BLUE, OUTPUT);
  pinMode(COL_WHITE, OUTPUT);

  digitalWrite(COL_RED, LOW);
  digitalWrite(COL_BLUE, LOW);
  digitalWrite(COL_WHITE, LOW);

  for (uint8_t x = 1; x < 15; x++) {
    digitalWrite(COL_RED, HIGH);
    delay(200-(x*8));
    digitalWrite(COL_RED, LOW);
    digitalWrite(COL_WHITE, HIGH);
    delay(12*x);
    digitalWrite(COL_WHITE, LOW);
    digitalWrite(COL_BLUE, HIGH);
    delay(x*x);
    digitalWrite(COL_BLUE, LOW);
  }
  
}


void loop() {

  // Debounce UPS input
  if ( (millis() - upsDebounce) > debounceDelay) {

    // If UPS status has changed from mains to battery power
    if (digitalRead(UPS_BATTERY) == HIGH && !isBattery) {

      delay(UPS_BAT_TIMEOUT);   // DeBounce UPS input

      if (digitalRead(UPS_BATTERY) == HIGH) {
        isBattery = true;
        curState = RED;
        counter = 0;
        upsDebounce = millis();
      }
      
    // If UPS status has changed from battery to mains power
    } else if (digitalRead(UPS_BATTERY) == LOW && isBattery) {
      isBattery = false;
      curState = getButtonValue();
      upsDebounce = millis();
    }
  }

  if (!isBattery)
    curState = getButtonValue();

  switch (curState) {
    case OFF:
      digitalWrite(COL_RED, LOW);
      digitalWrite(COL_BLUE, LOW);
      digitalWrite(COL_WHITE, LOW);
      break;

    case RED_BLUE:
      curState = RED + getButtonValue();
      
      if (++counter < FLASH_RATE) {
        digitalWrite(COL_RED, HIGH);
        digitalWrite(COL_BLUE, HIGH);
        digitalWrite(COL_WHITE, LOW);
        break;
      }
      
    case BLUE:
      digitalWrite(COL_RED, LOW);
      digitalWrite(COL_BLUE, HIGH);
      digitalWrite(COL_WHITE, LOW);
      break;

    case RED_WHITE:
      curState = RED + getButtonValue();
      
      if (++counter < FLASH_RATE) {
        digitalWrite(COL_RED, HIGH);
        digitalWrite(COL_BLUE, LOW);
        digitalWrite(COL_WHITE, HIGH);
        break;
      }
      
    case WHITE:
      digitalWrite(COL_RED, LOW);
      digitalWrite(COL_BLUE, LOW);
      digitalWrite(COL_WHITE, HIGH);
      break;

    case RED:
      if (++counter < FLASH_RATE) {
        digitalWrite(COL_RED, HIGH);
        digitalWrite(COL_BLUE, LOW);
        digitalWrite(COL_WHITE, LOW);
      } else {
        digitalWrite(COL_RED, LOW);
        digitalWrite(COL_BLUE, LOW);
        digitalWrite(COL_WHITE, LOW);
      }
      
      
      if (prevButton == OFF)
        curState = RED + getButtonValue();
      else
        getButtonValue();
      
      break;
    
  }
  
  if (counter >= (3 * FLASH_RATE))
    counter = 0;


  delay(5);
}


uint8_t getButtonValue(void) {
  // Debounce button input
  if ( (millis() - butDebounce) > debounceDelay) {

    // If button status has changed to white
    if (digitalRead(SW_WHITE) == HIGH && prevButton != WHITE) {
      prevButton = WHITE;
      butDebounce = millis();
    
    // If button status has changed to blue
    } else if (digitalRead(SW_BLUE) == HIGH && prevButton != BLUE) {
      prevButton = BLUE;
      butDebounce = millis();
    
    // If button status has changed to off
    } else if (digitalRead(SW_WHITE) == LOW && digitalRead(SW_BLUE) == LOW && prevButton != OFF) {
      prevButton = OFF;
      butDebounce = millis();
    }
  }
  return prevButton;
}

