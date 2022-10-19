#include <Arduino.h>
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
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(vlaznost, INPUT);
  pinMode(4, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(temper, INPUT);
  pinMode(lampa, OUTPUT);
  pinMode(careta, OUTPUT);
  pinMode(vent, OUTPUT);
  pinMode(pompa, OUTPUT);
}

void loop()
{
  if (analogRead(fotores) < 900) {
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    digitalWrite(lampa, HIGH);
  }
  if (analogRead(fotores) > 900) {
    digitalWrite(13, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(lampa, LOW);
  }
  if (analogRead(vlaznost) < 750) {
    digitalWrite(pompa, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(2, LOW);
  }
  if (analogRead(vlaznost) > 750) {
    digitalWrite(pompa, LOW);
    digitalWrite(4, LOW);
    digitalWrite(2, HIGH);
  }
  if (analogRead(temper) < 200) {
    digitalWrite(careta, LOW);
    digitalWrite(vent, LOW);
    digitalWrite(lampa, LOW);
  }
  if (analogRead(temper) > 200) {
    digitalWrite(careta, HIGH);
    digitalWrite(vent, HIGH);
    digitalWrite(lampa, HIGH);
  }
  if (analogRead(temper) > 200 && analogRead(vlaznost) < 750){
    digitalWrite(careta, HIGH);
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
  }
  if (analogRead(temper) < 200 && analogRead(vlaznost) > 750){
   	digitalWrite(careta, LOW);
    digitalWrite(vent, LOW);
    digitalWrite(pompa, LOW);
  }
  delay(careta); // Delay a little bit to improve simulation performance
}
