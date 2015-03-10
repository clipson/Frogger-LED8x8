/*
 * Christina Lipson
 * March. 9, 2015
 *
 * An 8x8 Frogger game. 
 * Get the frog past the cars and trucks and into the 3 bins on each of the 3 levels.
 *
 */

#include <FrequencyTimer2.h>

// Levels 1, 2 and 3. Left column is the starting point for the frog.
#define field1 { \
    {0, 0, 0, 0, 0, 1, 1, 0}, \
    {0, 0, 1, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 0, 0, 0, 0, 1}, \
    {0, 0, 0, 0, 0, 1, 0, 0}, \
    {0, 0, 0, 0, 0, 1, 1, 0}, \
    {0, 0, 0, 1, 0, 1, 1, 1}, \
    {0, 1, 0, 1, 0, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 1, 0} \
}

#define field2 { \
    {0, 0, 0, 0, 0, 1, 1, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 0, 1, 1, 0, 0, 0, 1}, \
    {0, 0, 0, 1, 0, 0, 1, 0}, \
    {0, 0, 0, 0, 0, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 1, 1}, \
    {0, 1, 0, 1, 0, 1, 1, 0}, \
    {0, 0, 0, 1, 0, 1, 1, 0} \
}

#define field3 { \
    {0, 0, 1, 0, 0, 1, 1, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 1, 0, 0, 0, 1}, \
    {0, 1, 0, 1, 0, 0, 1, 0}, \
    {0, 0, 0, 0, 0, 1, 1, 0}, \
    {0, 0, 0, 1, 0, 1, 1, 1}, \
    {0, 1, 0, 1, 0, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 1, 0} \
}

// The X when you lose
#define X { \
    {1, 1, 0, 0, 0, 0, 1, 1}, \
    {1, 1, 1, 0, 0, 1, 1, 1}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {1, 1, 1, 0, 0, 1, 1, 1}, \
    {1, 1, 0, 0, 0, 0, 1, 1} \
}

// The check when you win
#define check { \
    {0, 0, 1, 1, 0, 0, 0, 0}, \
    {0, 1, 1, 0, 0, 0, 0, 0}, \
    {1, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 0, 0, 0, 0, 0}, \
    {0, 0, 1, 1, 0, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 0, 1, 1, 0, 0}, \
    {0, 0, 0, 0, 0, 1, 1, 0} \
}

#define one { \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 1, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
}

#define two { \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 1, 1, 0, 0}, \
    {0, 1, 1, 0, 0, 1, 1, 0}, \
    {0, 1, 1, 1, 0, 0, 1, 0}, \
    {0, 1, 0, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 1, 1, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
}

#define three { \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 1, 0, 1, 0}, \
    {0, 1, 0, 1, 1, 1, 1, 0}, \
    {0, 0, 1, 1, 0, 1, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
}

byte leds[8][8]; // Background matrix
byte frog[8][8]; // Frog matrix

// Frog coordinates
int frogx = 4;
int frogy = 0;

int play = 0; // Condition to pull out of gameplay loop
int level = 1; // Level
int del = 10; // Loop delay (smaller as level increases)
int time = 0; // Used to loop display
byte col = 0; // Used to loop through leds

// 3 bins (start empty)
int leftBin = 0;
int middleBin = 0;
int rightBin = 0;

int buttonState1;             // the current reading from the input pin
int lastButtonState1 = LOW;   // the previous reading from the input pin
int buttonState2;            
int lastButtonState2 = LOW;

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime1 = 0;  // the last time the output pin was toggled
long debounceDelay1 = 50;    // the debounce time
long lastDebounceTime2 = 0;  
long debounceDelay2 = 50;

// pin[xx] on led matrix connected to nn on Arduino (-1 is dummy to make array start at pos 1)
int pins[17]= {-1, 4, 6, A5, 7, 10, 8, 9, 3, A4, A0, 12, A1, A2, A3, 5, 11};

// col[xx] of leds = pin yy on led matrix
int cols[8] = {pins[9], pins[14], pins[8], pins[12], pins[1], pins[7], pins[2], pins[5]};

// row[xx] of leds = pin yy on led matrix
int rows[8] = {pins[16], pins[15], pins[11], pins[06], pins[10], pins[4], pins[3], pins[13]};

// Buttons
const int up = 2;
const int left = 13;

// Assign background matrices
byte background1[8][8] = field1 ;
byte background2[8][8] = field2 ;
byte background3[8][8] = field3 ;

// Assign indication matrices
byte loss[8][8] = X ;
byte winner[8][8] = check ;
byte first[8][8] = one ;
byte second[8][8] = two ;
byte third[8][8] = three ;

void setup() {
  // sets the pins as output
  for (int i = 1; i <= 16; i++) {
    pinMode(pins[i], OUTPUT);
  }

  // set up cols and rows
  for (int i = 1; i <= 8; i++) {
    digitalWrite(cols[i - 1], LOW);
  }

  for (int i = 1; i <= 8; i++) {
    digitalWrite(rows[i - 1], LOW);
  }

  clearLeds();

  // Set interrupt routine to be called
  FrequencyTimer2::setOnOverflow(display);
}

// Main loop
void loop() {
  
  start();
  play = 0;
  while (play == 0) {
    
    // Read button states
    int reading1 = digitalRead(up);
    int reading2 = digitalRead(left);

    // check to see if you just pressed a button LOW to HIGH that is not noise

    // If the switch changed:
    if (reading1 != lastButtonState1) {
      // reset the debouncing timer
      lastDebounceTime1 = millis();
    } 
    if (reading2 != lastButtonState2) {
      // reset the debouncing timer
      lastDebounceTime2 = millis();
    } 

    if ((millis() - lastDebounceTime1) > debounceDelay1) {
      // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:
  
      // if the up button state has changed:
      if (reading1 != buttonState1) {
        buttonState1 = reading1;
  
        // only move the frog up if the new button state is HIGH
        if (buttonState1 == HIGH) {
          moveFrogUp();
        }
      }
    }
    if ((millis() - lastDebounceTime2) > debounceDelay2) {
       // whatever the reading is at, it's been there for longer
      // than the debounce delay, so take it as the actual current state:

      // if the left button state has changed:
      if (reading2 != buttonState2) {
        buttonState2 = reading2;

        // only move the frog left if the new button state is HIGH
        if (buttonState2 == HIGH) {
          moveFrogLeft();
        }
      }
    }
 
    // save the reading.  Next time through the loop,
    // they will be the lastButtonStates:
    lastButtonState1 = reading1;
    lastButtonState2 = reading2;
  
    // Slide pattern interval
    if (time % 100 == 0)
      slidePattern();
    
    //increment time (while loop number)
    time++;
    
    // save memory
    if (time >= 100000)
      time = 0;
    // display interval
    if (time % 100 == 0)
      display();
    
    // check if frog hits obstacle
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        if (leds[i][j] == HIGH && frog[i][j] == HIGH) {
          lose();
        }
      }
    }
  
    // check if frog makes it into safe zone
     for (int j = 0; j < 8; j++) {
        if (frog[j][7] == HIGH) {
          miniWin();
        }
      }
    
    // slide/display delay
    delay(del);
  }
}

void start() {
  // Level number
  levelNum();
  display();
  delay(1500);
  
  // Initialize background and frog
  setPattern();
  resetFrog();
  resetBins();
  display();
}

void clearLeds() {
  // Clear display array
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = 0;
    }
  }
}

void levelNum() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (level == 1) {
        leds[i][j] = first[i][j];
      }
      if (level == 2) {
        leds[i][j] = second[i][j];
      }
      if (level == 3) {
        leds[i][j] = third[i][j];
      }
    }
  }
}

// Set background led matrix
void setPattern() {
  
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (level == 1) {
        del = 10;
        leds[i][j] = background1[i][j];
      }
      if (level == 2) {
        del = 7;
        leds[i][j] = background2[i][j];
      }
      if (level == 3) {
        del = 5;
        leds[i][j] = background3[i][j];
      }
    }
  }
}

// set frog back to start position
void resetFrog() {
  frogx = 4;
  frogy = 0;
  frog[frogx][frogy] = 1;
}

// moves frog led position left by one
void moveFrogLeft() {
  frog[frogx][frogy] = 0;
  
  // wrap around if at the edge
  if (frogx == 0) {
    frog[7][frogy] = 1;
    frogx = 7;    
  }
  else {
    frog[frogx - 1][frogy] = 1;
    frogx = frogx - 1;
  }
}

// moves frog led position up by one
void moveFrogUp() {
  frog[frogx][frogy] = 0;
  frog[frogx][frogy + 1] = 1;
  frogy = frogy + 1;
}

// slides car and truck obstacles
void slidePattern() {
    
  // car slide
  for (int i = 1; i < 4; i++) {
    for (int j = 0; j < 8; j++) {
      if  (j == 7) {
        leds[7][i] = leds[0][i];
      }
      else {
        leds[j][i] = leds[j+1][i];
      }
    }
  }
  
  // truck slide
  for (int i = 5; i < 7; i++) {
    for (int j = 0; j < 8; j++) {
      if  (j == 7) {
        leds[7][i] = leds[0][i]; 
      }
      else {
        leds[j][i] = leds[j+1][i];
      }
    }
  }
}

// light led and frog matrices based on current patterns
void display() {
  
  digitalWrite(cols[col], LOW);  // Turn whole previous column off
  col++;
  if (col == 8) {
    col = 0;
  }
  
  //write background
  for (int row = 0; row < 8; row++) {
      if (leds[col][7 - row] == 1) {
        digitalWrite(rows[row], LOW); 
      }
      else {
        digitalWrite(rows[row], HIGH); 
      }
    
  }
  
  //write frog position
  for (int row = 0; row < 8; row++) {
      if (frog[col][7 - row] == 1) {
        digitalWrite(rows[row], LOW);  // Turn on this led
      }   
  }

  digitalWrite(cols[col], HIGH); // Turn whole column on at once (for equal lighting times)
}

// notify user of loss and reset level
void lose() {
  play = 1;
  frog[frogx][frogy] = 0;
  frogx = 0;
  frogy = 0;
  
  // assign led matrix to loss pattern (X)
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = loss[i][j];
    }
  }
  display();
  delay(2000);
}

// fill bin that the frog reached and reset frog position
void miniWin() {
  
  frog[frogx][frogy] = 0;
  
  if (frogx == 0 || frogx == 1 || leftBin == 1) {
    leds[0][7] = 1;
    leds[1][7] = 1;
    leftBin = 1;
  }
  if (frogx == 3 || frogx == 4 || middleBin == 1) {
    leds[3][7] = 1;
    leds[4][7] = 1;
    middleBin = 1;
  }
  if (frogx == 6 || frogx == 7 || rightBin == 1) {
    leds[6][7] = 1;  
    leds[7][7] = 1; 
    rightBin = 1;
  }
  
  // if all bins lit, user wins the level
  if (leftBin == 1 && middleBin == 1 && rightBin == 1) {
    win();
  }
  else {
    resetFrog();
    display();
  }
}

// notify user of win and initialize next level
void win() {
  play = 1;
  frog[frogx][frogy] = 0;
  frogx = 0;
  frogy = 0;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      leds[i][j] = winner[i][j];
    }
  }
  display();
  delay(2000);
  
  // increment level (if level 3, go back to level 1)
  if (level == 3) level = 1;
  else { level++; }
}

// reset bins to zero
void resetBins() {
  leftBin = 0;
  middleBin = 0;
  rightBin = 0;
}

