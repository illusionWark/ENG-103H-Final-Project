#include <Adafruit_CircuitPlayground.h>

const byte offPin = 7;//sets the interrupt pin
int switchState = 0;//switch state for interrupt
int score = 0;//for the main scoring system
int selectedLight = 0;//selecting which LED is going to be required to win 
int randDirect = 0;//selecting which way the LED will travel
int currentSweepLED = -1;//for checking the selected LED is the same as which LED is current on
bool roundOver = false;//used for preventing multiple triggers
volatile bool offFlag = false;//used as an off switch

uint8_t prevRed[10]; //Array for holding previous LED values so they don't get overridden 
uint8_t prevGreen[10];
uint8_t prevBlue[10];

void setup() { //Basic startup
  Serial.begin(9600);
  CircuitPlayground.begin();
  pinMode(offPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(offPin), off, CHANGE);
  while(!Serial); //wait to begin until serial is open
  winSelect(); //Selects an LED at random as the "winning" one
  randDirect = rand() % 2; //randomly selects one of two directions
}

void loop() {
  if (offFlag) { //Used to have an "off" state
    delay(5);
    CircuitPlayground.clearPixels();
    Serial.print("Stopped");
  } else {
      roundOver = false;  // reset for new round
      for (int i = 0; i < 10; i++) {
        if (i == selectedLight) {
          CircuitPlayground.setPixelColor(i, 0, 0, 255); // Keep the selected LED blue
        } else {
          CircuitPlayground.setPixelColor(i, 255, 255, 255); // White for others
        }
      }
      for (int i = 0; i < 10; i++) { //Save the colors of the LEDs in the array
        uint32_t color = CircuitPlayground.strip.getPixelColor(i);
        prevRed[i] = (uint8_t)(color >> 16);
        prevGreen[i] = (uint8_t)(color >> 8);
        prevBlue[i] = (uint8_t)(color);
      } 

      if (randDirect == 0) { //If the random direction is 0, go through this
          for (int i = 0; i < 10; i++) {
            currentSweepLED = i; //Checking where the LED is currently at
            CircuitPlayground.setPixelColor(i, 0, 255, 0);
            delay(150);
            CircuitPlayground.setPixelColor(i, prevRed[i], prevGreen[i], prevBlue[i]); //Rewrites the LED colors
            if(CircuitPlayground.leftButton()){ //If the left or right button is pressed, it will go to the checkWin function
              checkWin(0);
            } else if(CircuitPlayground.rightButton()){
              checkWin(1);
            }
          } 
      } else if (randDirect == 1) { //If the random direction is 1, go through this
          for (int i = 9; i >= 0; i--) {
            currentSweepLED = i;
            CircuitPlayground.setPixelColor(i, 0, 255, 0);
            delay(150);
            CircuitPlayground.setPixelColor(i, prevRed[i], prevGreen[i], prevBlue[i]);
            if(CircuitPlayground.leftButton()){
              checkWin(0);
            } else if(CircuitPlayground.rightButton()){
              checkWin(1);
            }
          } 
      }
  }
}

void winSelect() { //for selecting a random LED to be the winning one
  selectedLight = rand() % 10;
}

void checkWin(int buttonDirection) {
  if (roundOver) return;  // prevent multiple triggers
    roundOver = true;
  // Only trigger win if buttonDirection matches the sweep direction
  if (buttonDirection == randDirect && currentSweepLED == selectedLight) {
    winCon(); 
    Serial.println("Win!");
  } else {
    loseCon(); // Wrong button
    Serial.println("Wrong button - Lose!");
  }
}

void loseCon() {
  CircuitPlayground.playTone(50, 200);
  CircuitPlayground.playTone(40, 250);

  // Flash red a few times to indicate a loss
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 10; j++) {
      CircuitPlayground.setPixelColor(j, 255, 0, 0); // Red
    }
    delay(200);
    CircuitPlayground.clearPixels();
    delay(200);
  }

  score = 0;
  winSelect();               // Pick a new target LED
  randDirect = rand() % 2;   // Pick a new direction
  CircuitPlayground.clearPixels();
  delay(200);
}

void winCon() {
  CircuitPlayground.playTone(200, 200); //Plays a short sound
  CircuitPlayground.playTone(300, 200);
  score++;
  for(int i = 0; i < score; i++){ //Displays score using builtin LED
    CircuitPlayground.redLED(HIGH);
    delay(200);
    CircuitPlayground.redLED(LOW);
    delay(100);
  }
  winSelect();
  randDirect = rand() % 2;
  CircuitPlayground.clearPixels();
  delay(200);
}

void off() {
  offFlag = !offFlag;
}