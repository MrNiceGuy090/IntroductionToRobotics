const int pinA = 2;
const int pinB = 3;
const int pinC = 4;
const int pinD = 5;
const int pinE = 6;
const int pinF = 7;
const int pinG = 8;

const int EMFPin = A5;
const int buzzerPin = 10;

const int segSize = 7;
const int noOfDigits = 10;

const int sample = 500;
const int EMFRange = 25c;
const int buzzerDefaultIntervalDuration = 800;
const int buzzerDefaultFrequency = 440;
const int buzzerToneDuration = 50;
const int buzzerIntervalDurationMultiplier = 80;
const int buzzerFrequencyMultiplier = 100;
int buzzerIntervalDuration = 0;
long buzzerInterEMFValueTime = 0;
int index = 0;
float EMFValue;

bool digitMatrix[noOfDigits][segSize] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG
}; 

int averagingArray[sample];                   
unsigned long averaging;         

void displayNumber(byte digit){
  for(int i = 0; i < segSize; i++){
    digitalWrite(segments[i], digitMatrix[digit][i]);
    }
  }

void setup() {
  // put your setup code here, to run once:

  for(int i = 0; i < segSize; i++){
      pinMode(segments[i], OUTPUT);
    }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
    for(int i = 0; i < sample; i++){              
       averagingArray[i] = analogRead(EMFPin);   
       averaging += averagingArray[i];                     
    }                                                               
    EMFValue = averaging / sample;    
    Serial.println(EMFValue);                                                                  
    if(EMFValue >= 1){
      EMFValue = constrain(EMFValue, 1, EMFRange);                                                   
      EMFValue = map(EMFValue, 1, EMFRange, 0, 9);                                         
     }else{                                                                                                     
      EMFValue = 0;
    }

    buzzerIntervalDuration = buzzerDefaultIntervalDuration - EMFValue * buzzerIntervalDurationMultiplier;
    if(millis() - buzzerInterEMFValueTime > buzzerIntervalDuration){
      tone(buzzerPin, buzzerDefaultFrequency + EMFValue*buzzerFrequencyMultiplier, buzzerToneDuration);  
      buzzerInterEMFValueTime = millis();
    }
    displayNumber(EMFValue);
    averaging = 0;

      
}
