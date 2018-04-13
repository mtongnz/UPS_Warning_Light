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

enum states {
  OFF,
  WHITE,
  BLUE,
  RED,
  RED_WHITE,
  RED_BLUE
};

uint8_t curState = OFF, prevButton;
bool isBattery = false;
uint16_t counter = 0;


void setup() {
  pinMode(UPS_BATTERY, INPUT);
  pinMode(SW_WHITE, INPUT);
  pinMode(SW_BLUE, INPUT);
  pinMode(COL_RED, OUTPUT);
  pinMode(COL_BLUE, OUTPUT);
  pinMode(COL_WHITE, OUTPUT);

  digitalWrite(COL_RED, HIGH);
  digitalWrite(COL_BLUE, HIGH);
  digitalWrite(COL_WHITE, HIGH);
}


void loop() {

  // Check if we're on battery
  if (digitalRead(UPS_BATTERY) == LOW) {
    delay(5);
    if (digitalRead(UPS_BATTERY) == LOW) {
      if (!isBattery) {
        isBattery = true;
        curState = RED;
        counter = 0;
        prevButton = getButtonValue();
      }
    } else {
      isBattery = false;
      curState = getButtonValue();
    }
  } else {
    isBattery = false;
    curState = getButtonValue();
  }

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
        prevButton = getButtonValue();
      
      break;
    
  }
  
  if (counter >= (3 * FLASH_RATE))
    counter = 0;

}


uint8_t getButtonValue(void) {
  if (digitalRead(SW_WHITE) == HIGH && digitalRead(SW_BLUE) == HIGH) {
    delay(5);
    if (digitalRead(SW_WHITE) == HIGH && digitalRead(SW_BLUE) == HIGH)
      return OFF;
  } else if (digitalRead(SW_WHITE) == LOW) {
    delay(5);
    if (digitalRead(SW_WHITE) == LOW)
      return WHITE;
  } else if (digitalRead(SW_BLUE) == LOW) {
    delay(5);
    if (digitalRead(SW_BLUE) == LOW)
      return BLUE;
  } else
    delay(5);

}

