#include <SoftPWM.h>
#include <SoftPWM_timer.h>

struct RGB{
  byte red;
  byte green;
  byte blue;
};

struct RGBLED{
  RGB rgb;
  int redPin;
  int greenPin;
  int bluePin;
};


RGBLED topLED = {{0,0,0},2,4,3};
RGBLED bottomLED {{0,0,0},5,7,6};

int DELAY = 5;

int timer = 0;

int RED_LED_PIN = 8;
boolean redLed = false;


RGB colorsForLed[] = {{255,0,0},{0,0,255},{255,255,0},{0,255,255},{255,0,255},{0,125,255},{255,125,0},{255,255,255}};
int iterColor1 = 0;
int iterColor2 = 0;


void setup() {
  Serial.begin(9600);
  Serial.println("Serial Init.");
  // put your setup code here, to run once:
  pinMode(RED_LED_PIN, OUTPUT);
  SoftPWMBegin();
  SoftPWMSet(topLED.rgb.red, 0);
  SoftPWMSet(topLED.rgb.green, 0);
  SoftPWMSet(topLED.rgb.blue, 0);
  SoftPWMSet(bottomLED.rgb.red, 0);
  SoftPWMSet(bottomLED.rgb.green, 0);
  SoftPWMSet(bottomLED.rgb.blue, 0);
  
}


int fadeColor(RGB target,RGBLED* selectedLED, int fadeSpeed){
    int newRed = stepToValue(target.red,(selectedLED -> rgb).red, fadeSpeed);
    int newGreen = stepToValue(target.green,(selectedLED -> rgb).green, fadeSpeed);
    int newBlue = stepToValue(target.blue,(selectedLED -> rgb).blue , fadeSpeed);
    
    
    if(newRed == -1 && newGreen == -1 && newBlue == -1){
      return 1;
    }
    
    (selectedLED -> rgb).red = (newRed == -1) ? (selectedLED -> rgb).red : newRed;
    (selectedLED -> rgb).green = (newGreen == -1) ? (selectedLED -> rgb).green : newGreen;
    (selectedLED -> rgb).blue = (newBlue == -1) ? (selectedLED -> rgb).blue : newBlue;
    
    updateLED(selectedLED);
    return 0;
}

void updateLED(RGBLED* selectedLED){
  SoftPWMSet(selectedLED ->redPin , 255 - (selectedLED -> rgb).red);
  SoftPWMSet(selectedLED -> greenPin, 255 - (selectedLED -> rgb).green);
  SoftPWMSet(selectedLED -> bluePin, 255 - (selectedLED -> rgb).blue);
}

int stepToValue(byte target, byte value, byte fadeSpeed){
  if(value > target){
      return (value - fadeSpeed);
    }else if(value < target){
      return (value + fadeSpeed);
    }else{
      //Serial.println("********************************");
      return -1;
    }
}

void loop() {
  if(timer > 499){
    timer = 0;
  }
  if(iterColor1 > 7){
    iterColor1 = 0;
  }
  if(iterColor2 > 7){
    iterColor2 = 0;
  }
  if((timer % 250) == 0){
    if(redLed){
      digitalWrite(RED_LED_PIN, LOW);
      redLed = false;
    }else{
      digitalWrite(RED_LED_PIN, HIGH);
      redLed = true;
    }
  }else{
    
    if(fadeColor(colorsForLed[iterColor1],&topLED,1)){
      iterColor1++;
    }
    if(fadeColor(colorsForLed[7 - iterColor2],&bottomLED,1)){
      iterColor2++;
    }
    
  }
  timer++;
  
  
  delay(DELAY);
}
