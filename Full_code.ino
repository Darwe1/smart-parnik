#include <Arduino.h>
#include <GyverStepper.h>
GStepper<STEPPER2WIRE> stepper(2048, 12, 11, 13);
#define 10 careta
#define 9 vent
#define 8 pompa
#define 7 lampa
#define A0 fotores
#define A1 temper
#define A2 vlaznost

void setup()
{
  pinMode(fotores, INPUT);
  pinMode(vlaznost, INPUT);
  pinMode(temper, INPUT);
  pinMode(lampa, OUTPUT);
  pinMode(careta, OUTPUT);
  pinMode(vent, OUTPUT);
  pinMode(pompa, OUTPUT);
  Serial.begin(9600);
  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(100);
}

void lamp(){
  if(analogRead(fotores) < 900){
    digitalWrite(lampa, HIGH);
  }
  else
  {
    digitalWrite(lampa, LOW);
  }
}

void pomp(){
    if (analogRead(vlaznost) < 750) {
    digitalWrite(pompa, HIGH);
  }
  if (analogRead(vlaznost) > 750) {
    digitalWrite(pompa, LOW);
  }
}


////принимает число 1-3 и выставляется 



void caret(){
  if (analogRead(temper) < 200 && < 500) {
    stepper.setTarget(0);
    digitalWrite(vent, LOW);
  }
  if (analogRead(temper) > 200 && < 500) {
    stepper.setTarget(150);
    digitalWrite(vent, HIGH);
  }
  if(analogRead(temper) > 500) {
    stepper.setTarget(300);
    digitalWrite(vent, HIGH);  
  }
}

void caret_pomp();{
    if(analogRead(temper) > 200 && analogRead(vlaznost) < 750){
    resume();
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
  }
  if (analogRead(temper) < 200 && analogRead(vlaznost) > 750){
    pause();
    digitalWrite(vent, LOW);
    digitalWrite(pompa, LOW);
  }
  if (analogRead(temper) > 200 && analogRead(vlaznost) > 750){
    resume();
    digitalWrite(vent,HIGH);
    digitalWrite(pompa, LOW);
  }
}

void 
void loop()
{
  lamp();
  pomp();
  caret();
  caret_pomp();
}
