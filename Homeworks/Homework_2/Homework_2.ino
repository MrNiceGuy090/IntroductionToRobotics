const int pedestrianRedPin = 5;
const int pedestrianGreenPin = 6;

const int autoGreenPin = 8;
const int autoYellowPin = 9;
const int autoRedPin = 10;

const int buttonPin = 2;
const int buzzerPin = 3;

bool buttonState = HIGH;

bool pedestrianRedState = HIGH;
bool pedestrianGreenState = LOW;
bool autoGreenState = HIGH;
bool autoYellowState = LOW;
bool autoRedState = LOW;

unsigned long lastDebounceTime =0;
unsigned long buttonPressedAcknowledgedTime = 0;
unsigned int debounceDelay = 50;
unsigned int buttonEffectDelay = 10000;
unsigned int state2Duration = 3000;
unsigned int state3Duration = 10000;
unsigned int state4Duration = 3600;
unsigned long pedestrianGreenFlashTime = 0;
unsigned long pedestrianGreenFlashDuration = 400;
unsigned long buzzerSmallIntervalTime = 0;
unsigned int buzzerSmallIntervalDuration = 1000;

bool isPedestrianGreenFlashing = LOW;
bool hasButtonBeenPressed = LOW;

bool isInState1 = HIGH;
bool isInState2 = LOW;
bool isInState3 = LOW;
bool isInState4 = LOW;

int reading = HIGH;
int previousReading = HIGH;

void setup() {
  // put your setup code here, to run once:
  pinMode(pedestrianRedPin,OUTPUT);
  pinMode(pedestrianGreenPin,OUTPUT);
  
  pinMode(autoGreenPin,OUTPUT);
  pinMode(autoYellowPin,OUTPUT);
  pinMode(autoRedPin,OUTPUT);
  
  pinMode(buttonPin,INPUT_PULLUP);
  Serial.begin(9600);

  digitalWrite(pedestrianRedPin, pedestrianRedState);
  digitalWrite(autoGreenPin, autoGreenState);
}

void loop() {
  // put your main code here, to run repeatedly:
  reading = digitalRead(buttonPin);
  if(reading != previousReading) {
    lastDebounceTime = millis();
  }
  
  if( (millis() - lastDebounceTime) > debounceDelay){
    if(reading != buttonState) {
      buttonState = reading;
      if(buttonState == HIGH && hasButtonBeenPressed == LOW) {
          buttonPressedAcknowledgedTime = millis();
          hasButtonBeenPressed = HIGH;
      }
    }
  }

  if(isInState1 && hasButtonBeenPressed && millis() - buttonPressedAcknowledgedTime > buttonEffectDelay){
    isInState1 = LOW;
    isInState2 = HIGH;
  }

  if (isInState2){
    if(autoGreenState == HIGH && autoYellowState == LOW){
      autoGreenState = !autoGreenState;
      autoYellowState = !autoYellowState;
      digitalWrite(autoGreenPin, autoGreenState);
      digitalWrite(autoYellowPin, autoYellowState);
    }
    if(millis() - buttonPressedAcknowledgedTime > buttonEffectDelay + state2Duration){
      isInState2 = LOW;
      isInState3 = HIGH;
    }
  }
  
  if(isInState3){
    if(pedestrianRedState == HIGH && pedestrianGreenState == LOW && autoYellowState == HIGH && autoRedState == LOW ){
      pedestrianRedState = !pedestrianRedState;
      pedestrianGreenState = !pedestrianGreenState;
      autoYellowState = !autoYellowState;
      autoRedState = !autoRedState;
  
      digitalWrite(pedestrianRedPin, pedestrianRedState);
      digitalWrite(pedestrianGreenPin, pedestrianGreenState);
      digitalWrite(autoYellowPin, autoYellowState);
      digitalWrite(autoRedPin, autoRedState); 
    }
    if(millis() - buzzerSmallIntervalTime > buzzerSmallIntervalDuration){    
      tone(buzzerPin, 1000,50);
      buzzerSmallIntervalTime = millis();
    } 
    if(millis() - buttonPressedAcknowledgedTime > buttonEffectDelay + state2Duration + state3Duration ){
      isInState3 = LOW;
      isInState4 = HIGH;  
    }
  }
      
  if(isInState4){
    if(isPedestrianGreenFlashing == LOW){
      pedestrianGreenFlashTime = millis();   
      tone(buzzerPin, 1000,50);
      isPedestrianGreenFlashing = HIGH;
    }
    if(millis() - pedestrianGreenFlashTime > pedestrianGreenFlashDuration){
      pedestrianGreenState = !pedestrianGreenState;
      digitalWrite(pedestrianGreenPin, pedestrianGreenState);
    
      tone(buzzerPin, 1000,50);
      pedestrianGreenFlashTime = millis();
    }
      
    if(millis() - buttonPressedAcknowledgedTime > buttonEffectDelay + state2Duration + state3Duration + state4Duration){
      pedestrianRedState = !pedestrianRedState;
      // pedestrianGreenState might be changed already to LOW in the flashing pedestrian green if
      pedestrianGreenState = LOW;
      autoRedState = !autoRedState;
      autoGreenState = !autoGreenState;
  
      digitalWrite(pedestrianRedPin, pedestrianRedState);
      digitalWrite(pedestrianGreenPin, pedestrianGreenState);
      digitalWrite(autoGreenPin, autoGreenState);
      digitalWrite(autoRedPin, autoRedState);
      
      isPedestrianGreenFlashing = LOW;
      hasButtonBeenPressed = LOW;
      isInState4 = LOW;
      isInState1 = HIGH;
    }
  }
  
  previousReading = reading;
}
