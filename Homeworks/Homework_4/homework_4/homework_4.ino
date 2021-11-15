// joyStick pins
const int pinSW = 9;
const int pinX = A0;
const int pinY = A1;

// 4 digit 7 display pins
const int dataPin = 12; //DS
const int latchPin = 11; //STCP
const int clockPin = 10; //SHCP

// joystick variables
bool swState =LOW;
bool lastSwState =LOW;
int xValue =0;
int yValue =0;
bool joyMoved = false;
int minThreshold=400;
int maxThreshold=600;

unsigned int lastDebounceTime =0;
unsigned int debounceDelay =50;

// 4 digit 7 display variables
bool isLockedInOnDigit = false;
int dpPosition = 3;
bool dpState = false;
int dpBlinkingInterval = 300;
long dpBlinkingTime = 0;
int ledNumber[4] = {1,2,3,4};
int numberLength = 4;

int digit;

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;

const int displayDigits[] = {
  segD1, segD2, segD3, segD4 
};

int digitArray[16] = {
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

void setup(){
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  for (int i = 0; i < displayCount; i++){
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  pinMode(pinSW,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop(){

  writeNumber(ledNumber, numberLength, dpPosition, isLockedInOnDigit);
  
  xValue = analogRead(pinX);
  if( xValue < minThreshold && joyMoved == false){
    if(digit >0) {
      digit--;
    }
    else{
      digit =9;
    }
    joyMoved = true;
  }
  if(xValue > maxThreshold && joyMoved == false){
    if(digit <9) {
      digit++;
    }
    else{
      digit =0;
    }
    joyMoved = true;
  }
  if(xValue >= minThreshold && xValue <= maxThreshold){
    joyMoved = false;
  }

  bool reading = digitalRead(pinSW);
  if(reading != lastSwState) {
    lastDebounceTime = millis();
  }
  if( (millis() - lastDebounceTime) > debounceDelay){
    if(reading != swState) {
      swState = reading;
      if(swState == HIGH) {
        if (isLockedInOnDigit == false){
          isLockedInOnDigit = true;
        }
        else{
          isLockedInOnDigit = false;
        }
      }
    }
  }

  lastSwState = reading;
}

void writeNumber(int number[],int numberLength, int displayPos, bool isSelected){
  bool dpSegments[4] = {false,false,false,false};
  dpSegments[displayPos] = true;
  for(int currentPos = 0; currentPos < numberLength; currentPos++){
      showDigit(numberLength - 1 - currentPos);
      if(currentPos == displayPos){
        if(isSelected == true){
          dpSegments[displayPos] = true;
        }
        else if( millis() - dpBlinkingTime > dpBlinkingInterval){
          dpSegments[displayPos] = !dpSegments[displayPos];
          dpBlinkingTime = millis(); 
        }
      }
      
        
      int digit = digitArray[number[currentPos]];
      if(dpSegments[currentPos] == true) {
        digit++;
      }
      writeRegister(digit);
      delay(1);
    }
}

void showDigit(int digit){
  for(int i = 0; i < displayCount; i++){
    digitalWrite(displayDigits[i], HIGH);
  };
  digitalWrite(displayDigits[digit], LOW);
}

void writeRegister(int digit){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  digitalWrite(latchPin, HIGH);
}
