///#include <IRremote.hpp>

#include <FIFO.h>
#include <GyverStepper.h>
#include <IRremote.h>

IRrecv ir(10);
GStepper<STEPPER2WIRE> stepper(2048, 12, 11, 13);
#define careta 10
#define vent 9
#define pompa 8
#define lampa 7
#define fotores A0
#define temper A1
#define vlaznost A2
#define on_off 0
long seed;
bool mi = 0; //работает ли устройство в ручном режиме
decode_results res;

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
  ir.enableIRIn();
  pinMode(5,OUTPUT);
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
  if (analogRead(temper) < 200) {
    stepper.setTarget(0);
    digitalWrite(vent, LOW);
  }
  if (analogRead(temper) > 200 && analogRead(temper) < 500) {
    stepper.setTarget(150);
    digitalWrite(vent, HIGH);
  }
  if(analogRead(temper) > 500) {
    stepper.setTarget(300);
    digitalWrite(vent, HIGH);  
  }
}

///caret_and_pomp(analogRead(temper), )
///void caret_and_pomp(int temp_val,int vlaznost_val)

void caret_pomp(){
  if(analogRead(temper) > 200 && analogRead(vlaznost) < 750){
    stepper.setTarget(150);
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
  }
  else if (analogRead(temper) < 200 && analogRead(vlaznost) > 750){
    stepper.setTarget(0);
    digitalWrite(vent, LOW);
    digitalWrite(pompa, LOW);
  }
  else if (analogRead(temper) > 200 && analogRead(vlaznost) > 750){
    stepper.setTarget(150);
    digitalWrite(vent,HIGH);
    digitalWrite(pompa, LOW);
  }
  else if (analogRead(temper) > 500 && analogRead(vlaznost) > 750){
    stepper.setTarget(300);
    digitalWrite(vent,HIGH);
    digitalWrite(pompa, LOW);
  }
  else if (analogRead(temper) > 500 && analogRead(vlaznost) < 750){
    stepper.setTarget(300);
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
  }
}

void hc(long seed){

}

void loop()
{
    if (ir.decode(& res)) {
    Serial.println(res.value);
    seed = res.value;
    ir.resume();
  }
  if(seed == on_off) //on_off значение типа long с пульта на включение/выключение
  {
    digitalWrite(5, !(digitalRead(5)));
    mi = !mi;
  }
  if(mi){
    hc(seed);
  }
  else
  {
  lamp();
  pomp();
  caret();
  caret_pomp();
  }
}