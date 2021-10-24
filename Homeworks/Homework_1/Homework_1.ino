const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;
const int redPotPin = A0;
const int greenPotPin = A1;
const int bluePotPin = A2;

const int maxColorValue = 255;
const int maxPotValue = 1023;

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;

int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redPotPin, INPUT);  
  pinMode(greenPotPin, INPUT);
  pinMode(bluePotPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  redPotValue = analogRead(redPotPin);
  greenPotValue = analogRead(greenPotPin);
  bluePotValue = analogRead(bluePotPin);

  redBrightness = map(redPotValue, 0, maxPotValue, 0, maxColorValue);
  greenBrightness = map(greenPotValue, 0, maxPotValue, 0, maxColorValue);
  blueBrightness = map(bluePotValue, 0, maxPotValue, 0, maxColorValue);
  
  analogWrite(redLedPin, redBrightness);
  analogWrite(greenLedPin, greenBrightness);
  analogWrite(blueLedPin, blueBrightness);
}
