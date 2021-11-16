#include <EEPROM.h>
// joyStick pins
const int pinSW = 3;
const int pinX = A0;
const int pinY = A1;

// joystick variables
int xValue =0;
int yValue =0;
bool joyMoved = false;
const int minThreshold=400;
const int maxThreshold=600;

// 4 digit 7 segment display pins
const int dataPin = 12; //DS
const int latchPin = 11; //STCP
const int clockPin = 10; //SHCP
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

// 4 digit 7 segment display constants
const int displayCount = 4;
const int displayDigits[] = {
  segD1, segD2, segD3, segD4 
};
const int digitArray[16] = {
//A B C D E F G DP 
  B11111100, // 0
  B01100000, // 1
  B11011010, // 2
  B11110010, // 3
  B01100110, // 4
  B10110110, // 5
  B10111110, // 6
  B11100000, // 7
  B11111110, // 8
  B11110110, // 9
  B11101110, // A
  B00111110, // b
  B10011100, // C
  B01111010, // d
  B10011110, // E
  B10001110, // F
};

// logic variables
volatile bool isLockedInOnDigit = false;
int currentPos = 3;
int dpBlinkingInterval = 200;
long dpBlinkingTime = 0;
int ledNumber[4] = {1,2,3,4};
int numberLength = 4;

void setup(){
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  for (int i = 0; i < displayCount; i++){
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(pinSW,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), buttonPressed, FALLING);
  if ( EEPROM.read(0) != 0xff )
    for (int i = 0; i < numberLength; ++i )
        ledNumber[i] = EEPROM.read(i);
  Serial.begin(9600);
}

void loop(){
  writeNumber(ledNumber, numberLength, currentPos, isLockedInOnDigit);

  if(isLockedInOnDigit == true){
    checkJoystickXAxisMovement(pinX, minThreshold, maxThreshold, joyMoved, ledNumber, currentPos );
  }
  else{
    checkJoystickYAxisMovement(pinY,minThreshold, maxThreshold, joyMoved, currentPos); 
  }
}


void checkJoystickXAxisMovement(const int pinX, const int minThreshold, const int maxThreshold, bool &joyMoved, int number[], int currentPos){
    xValue = analogRead(pinX);
    if( xValue < minThreshold && joyMoved == false){
      if(number[currentPos] >0) {
        number[currentPos]--;
      }
      else{
        number[currentPos] = 9;
      }
      joyMoved = true;
    }
    if(xValue > maxThreshold && joyMoved == false){
      if(number[currentPos] <9) {
        number[currentPos]++;
      }
      else{
        number[currentPos] = 0;
      }
      joyMoved = true;
    }
    if(xValue >= minThreshold && xValue <= maxThreshold){
      joyMoved = false;
    }
  }
  
void checkJoystickYAxisMovement(const int pinY, const int minThreshold,const  int maxThreshold, bool &joyMoved, int &currentPos){
    yValue = analogRead(pinY);
    if( yValue < minThreshold && joyMoved == false){
      if(currentPos > 0) {
        currentPos--;
      }
      else{
        currentPos = 3;
      }
      joyMoved = true;
    }
    if(yValue > maxThreshold && joyMoved == false){
      if(currentPos <3) {
        currentPos++;
      }
      else{
        currentPos = 0;
      }
      joyMoved = true;
    }
    if(yValue >= minThreshold && yValue <= maxThreshold){
      joyMoved = false;
    }
}


void buttonPressed(){
  isLockedInOnDigit = !isLockedInOnDigit;
  
  for (int i = 0; i < numberLength; ++i )
     EEPROM.write ( i, ledNumber[i]);
}

void writeNumber(int number[],int numberLength, int displayPos, bool isSelected){
  bool dpOn = false;
  for(int currentPos = 0; currentPos < numberLength; ++currentPos){
      dpOn = false;
      showDigit(numberLength - 1 - currentPos);
      if(currentPos == displayPos){
        if(isSelected == true){
          dpOn = true;
        }
        else if( millis() - dpBlinkingTime > dpBlinkingInterval){
          dpOn = !dpOn;
          dpBlinkingTime = millis(); 
        }
      }
      int digit = digitArray[number[currentPos]];
      if(dpOn == true) {
        digit++;
      }
      writeRegister(digit);
      delay(5);
    }
}

void showDigit(int digit){
  for(int i = 0; i < displayCount; ++i){
    digitalWrite(displayDigits[i], HIGH);
  };
  digitalWrite(displayDigits[digit], LOW);
}

void writeRegister(int digit){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}
