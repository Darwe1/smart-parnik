///#include <IRremote.hpp>
#include <LiquidCrystal.h>
#include <GyverStepper.h>
#include <IRremote.h>

LiquidCrystal lcd(40,38,36,34,32,30,28,26,24,22);
IRrecv ir(8);
GStepper<STEPPER2WIRE> stepper(2048, 12, 11, 13);
#define B1  0xFFE01F//каретка положение 1 -
#define B1_2 0xFFA857// каретка положение 2 +
#define B2  0xFF30CF//вентелятор на пульте 1
#define B3  0xFF18E7//помпа на пульте 2
#define B4  0xFF7A85// лампа на пульте 3
#define careta 10
#define vent 9
#define pomp 8
#define lamp 7
#define fotores A0
#define temper A1
#define vlaznost A2
#define on_off 0xFFA25D
long seed;
int n = 0;
bool mi = 0; //работает ли устройство в ручном режиме
decode_results res;

void setup()
{
  ///lcd.begin(16,2);
  pinMode(fotores, INPUT);
  pinMode(vlaznost, INPUT);
  pinMode(temper, INPUT);
  pinMode(lamp, OUTPUT);
  pinMode(careta, OUTPUT);
  pinMode(vent, OUTPUT);
  pinMode(pompa, OUTPUT);
  Serial.begin(9600);
  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(100);
  ir.enableIRIn();
  pinMode(5,OUTPUT);
  lcd.begin(16, 2); // выставляем скорость COM порта
  ///irrecv.enableIRIn(); // запускаем прием
  ///lcd.autoscroll();
}

void lampa(){
  lcd.clear();
  if(analogRead(fotores) < 900){
    ///Serial.println("lampa 1");
    digitalWrite(lampa, HIGH);
    lcd.print("Lampa 1");
    lcd.home();
  }
  else
  {
    ///Serial.println("lampa 0");
    digitalWrite(lampa, LOW);
    lcd.print("lampa 0");
    lcd.home();
  }
}

void pompa(){
  lcd.clear();
    if (analogRead(vlaznost) < 750) {
    ///Serial.println("pompa 1");
    digitalWrite(pompa, HIGH);
    lcd.print("Pomp 1");
    lcd.home();
  }
  if (analogRead(vlaznost) > 750) {
    ///Serial.println("pompa 0");
    digitalWrite(pompa, LOW);
    lcd.print("Pomp 0");
    lcd.home();
  }
}

void caret(){
  lcd.clear();
  if (analogRead(temper) < 200) {
    ///Serial.println("caret 0, vent 0");
    stepper.setTarget(0);
    digitalWrite(vent, LOW);
    lcd.print("Caret 0");
    lcd.home();
  }
  if (analogRead(temper) > 200 && analogRead(temper) < 500) {
    ///Serial.println("caret 1/2, vent 1");
    stepper.setTarget(150);
    digitalWrite(vent, HIGH);
  }
  if(analogRead(temper) > 500) {
    ///Serial.println("caret 1, vent 1");
    stepper.setTarget(300);
    digitalWrite(vent, HIGH);  
  }
}

///caret_and_pomp(analogRead(temper), )
///void caret_and_pomp(int temp_val,int vlaznost_val)

/*void caret_pomp(){
  lcd.clear();
  if(analogRead(temper) > 200 && analogRead(vlaznost) < 750){
    stepper.setTarget(150);
    ///Serial.print("caret 1/2 , vent 1 , pompa 1");
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
    lcd.print("Pos  Vent  Pomp");
    lcd.setCursor(1,2);
    lcd.print(" 1     1      1");
  }
  else if (analogRead(temper) < 200 && analogRead(vlaznost) > 750){
    stepper.setTarget(0);
    ///Serial.print("caret 0 , vent 0 , pompa 0");
    digitalWrite(vent, LOW);
    digitalWrite(pompa, LOW);
    lcd.print("Pos  Vent  Pomp");
    lcd.setCursor(1,2);
    lcd.print(" 0     0      0");
  }
  else if (analogRead(temper) > 200 && analogRead(vlaznost) > 750){
    stepper.setTarget(150);
    ///Serial.print("caret 1/2 , vent 1 , pompa 0");
    digitalWrite(vent,HIGH);
    digitalWrite(pompa, LOW);
    lcd.print("Pos  Vent  Pomp");
    lcd.setCursor(1,2);
    lcd.print(" 1     1      0");
  }
  else if (analogRead(temper) > 500 && analogRead(vlaznost) > 750){
    stepper.setTarget(300);
    ///Serial.print("caret 1 , vent 1 , pompa 0");
    digitalWrite(vent,HIGH);
    digitalWrite(pompa, LOW);
  }
  else if (analogRead(temper) > 500 && analogRead(vlaznost) < 750){
    stepper.setTarget(300);
    ///Serial.print("caret 1 , vent 1 , pompa 1");
    digitalWrite(vent, HIGH);
    digitalWrite(pompa, HIGH);
  }
}*/

void hc(long seed){
    if(seed == B2){
      if(digitalRead(9) == 0){
        digitalWrite(9, HIGH);
        lcd.clear();
        lcd.print("Vent robit");
        lcd.home();
      }
      else
      {
        digitalWrite(9, LOW);
        lcd.clear();
        lcd.print("Vent ne robit");
        lcd.home();
      }
    }
    if(seed == B3){
       if(digitalRead(8) == 0){
        digitalWrite(8, HIGH);
      }
      else
      {
        digitalWrite(8, LOW);
      }
    }
    if(seed == B4){
      if(digitalRead(7) == 0){
        digitalWrite(7, HIGH);
      }
      else
      {
        digitalWrite(7, LOW);
      }
    }
    if(seed == B1_2){
      if(n < 3){
        n += 1;
        //Serial.println("add");
      }
      if(n == 1){
        stepper.setTarget(150);
        lcd.clear();
        lcd.print("Position:");
        lcd.leftToRight();
        lcd.print(n);
        lcd.home();
        //Serial.println(1);
      }
      if(n == 2){
        stepper.setTarget(300);
        lcd.clear();
        lcd.print("Position:");
        lcd.leftToRight();
        lcd.print(n);
        lcd.home();
        //Serial.println(2);
      }
    }
    if(seed == B1){
      if(n > 0){
        n -= 1;
      }
      if(n == 0){
        stepper.setTarget(0);
        lcd.clear();
        lcd.print("Position:");
        lcd.leftToRight();
        lcd.print(n);
        lcd.home();
      }
      if(n == 1){
        stepper.setTarget(150);
        lcd.clear();
        lcd.print("Position:");
        lcd.leftToRight();
        lcd.print(n);
        lcd.home();
      }
    }
}

void loop()
{
  if (ir.decode(& res)) {
    Serial.println(res.value);
    seed = res.value;
    ///Serial.println(seed, HEX);
    delay(100);
    ir.resume();
  }
  if(seed == on_off) //on_off значение типа long с пульта на включение/выключение
  {
    digitalWrite(5, !(digitalRead(5)));
    mi = !mi;
    seed = 0;
  }
  if(mi){
    hc(seed);
    seed = 0;
    //Serial.println("hc");
  }
  else
  {
    Serial.print("ac");
    lampa();
    pompa();
    caret();
    ///caret_pomp();
  }
}   