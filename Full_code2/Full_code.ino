///#include <IRremote.hpp>
#include <LiquidCrystal.h>
#include <GyverStepper.h>
#include <IRremote.h>

LiquidCrystal lcd(40,38,36,34,32,30,28,26,24,22);
IRrecv ir(8);
GStepper<STEPPER2WIRE> stepper(2048, 10, 9, 11);
#define B1  0xFFE01F//каретка положение 1 -
#define B1_2 0xFFA857// каретка положение 2 +
#define B2  0xFF30CF//вентилятор на пульте 1
#define B3  0xFF18E7//помпа на пульте 2
#define B4  0xFF7A85// лампа на пульте 3
#define test 0xFF52AD//тест всех систем
#define B5 0xFF5AA5//нагреватель
#define nagrev 22
#define careta 10
#define vent 7
#define pomp 5
#define lamp 6
#define lamp2 2
#define fotores A0
#define temper A1
#define vlaznost A2
#define on_off 0xFFA25D
long seed;
int n = 0;
bool mi = 0; //работает ли устройство в ручном режиме
decode_results res;
///добавить флаг питания двигателя!

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
    digitalWrite(lamp2, HIGH);
    lcd.print("Lampa 1");
    lcd.home();
  }
  else
  {
    ///Serial.println("lampa 0");
    digitalWrite(lamp2, LOW);
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
    ///сделать так чтобы помпа качала определенное время  
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
  ///copy from hc
  if (analogRead(temper) < 200) {
    ///Serial.println("caret 0, vent 0");
    stepper.setTarget(0);
    digitalWrite(vent, LOW);
    lcd.print("Caret 0");
    lcd.SetCursor(1,2);
    lcd.print("Nagrev 1");
    lcd.home();
    digitalWrite(nagrev, HIGH);
    delay(10000);
    digitalWrite(nagrev, LOW);
    digitalWrite(vent, HIGH);
    delay(5000);
    digitalWrite(vent, LOW);
  }
  if (analogRead(temper) > 200 && analogRead(temper) < 500) {
    ///Serial.println("caret 1/2, vent 1");
    stepper.setTarget(150);
    digitalWrite(vent, LOW);
    lcd.print("Caret 1/2");
    lcd.home();
  }
  if(analogRead(temper) > 500) {
    ///Serial.println("caret 1, vent 1");
    stepper.setTarget(300);
    digitalWrite(vent, HIGH);  
    lcd.print("Caret 1");
    lcd.home();
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
  ///if (seed == B1_2 || seed == B1) {
    ////stepper.enable();
  ///}
    if(seed == B2){
      if(digitalRead(7) == 0){
        digitalWrite(7, HIGH);
        lcd.clear();
        lcd.print("Vent robit");
        lcd.home();
      }
      else
      {
        digitalWrite(7, LOW);
        lcd.clear();
        lcd.print("Vent ne robit");
        lcd.home();
      }
    }
    if(seed == B3){
       if(digitalRead(5) == 0){
        digitalWrite(5, HIGH);
        lcd.clear();
        lcd.print("Pomp robit");
        lcd.home();
      }
      else
      {
        digitalWrite(5, LOW);
        lcd.clear();
        lcd.print("Pomp robit");
        lcd.home();
      }
    }
    if(seed == B4){
      if(digitalRead(2) == 0){
        digitalWrite(2, HIGH);
        lcd.clear();
        lcd.print("Lamp robit");
        lcd.home();
      }
      else
      {
        digitalWrite(2, LOW);
        lcd.clear();
        lcd.print("Lamp robit");
        lcd.home();
      }
    }
    if(seed == B5){
      if(digitalRead(22) == 0){
        digitalWrite(22, HIGH);
        lcd.clear();
        lcd.print("Nagrev robit");
        lcd.home();
      }
      else
      {
        digitalWrite(22, LOW);
        lcd.clear();
        lcd.print("Nagrev robit");
        lcd.home();
      }
    }
    if(seed == test){
      lcd.clear();
      lcd.print("test started");
      lcd.home();
      delay(500);
      digitalWrite(2, HIGH);
      lcd.clear();
      lcd.print("Lamp robit");
      delay(1000);
      lcd.home();
      lcd.clear();
      lcd.print("Lamp ne robit");
      delay(500);
      digitalWrite(2, lOW);
      lcd.home();
      lcd.clear();
      lcd.print("Vent robit");
      digitalWrite(7, HIGH);
      delay(1000);
      digitalWrite(7, LOW)
      lcd.home();
      lcd.clear();
      lcd.print("Vent ne robit");
      delay(500);
      lcd.home();
      lcd.clear();
      lcd.print("Pomp robit");
      digitalWrite(5, HIGH);
      delay(500);
      digitalWrite(5, LOW)
      lcd.home();
      lcd.clear();
      lcd.print("Pomp ne robit");
      delay(500);
      lcd.home();
      lcd.clear();
      lcd.print("Test end");
      lcd.home();
      lcd.clear();
      lcd.print("Have a nice day!");
    }
    if(!stepper.tick()){
     if(seed == B1_2){
        if(n < 3){
        n += 1;
        //Serial.println("add");
      }
       if(n == 1){
        //stepper.enable();
        stepper.setTarget(150, RELATIVE);
        ///delay(10000);
        //stepper.disable();
        lcd.clear();
        lcd.print("Position:");
        lcd.leftToRight();
        lcd.print(n);
        lcd.home();
        //Serial.println(1);
      }
        if(n == 2){
       // stepper.enable();
        stepper.setTarget(150, RELATIVE);
        ///stepper.disable();
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
      
        if(n == 0){
        // stepper.enable();
          stepper.setTarget(-150, RELATIVE);
          ////stepper.disable();
          lcd.clear();
          lcd.print("Position:");
          lcd.leftToRight();
          lcd.print(n);
          lcd.home();
        }
        if(n == 1){
        // stepper.enable();
          stepper.setTarget(-150, RELATIVE);
          ///delay(10000);
        // stepper.disable();
          lcd.clear();
          lcd.print("Position:");
          lcd.leftToRight();
          lcd.print(n);
          lcd.home();
        }
      }
    }
  }  
}

void loop()
{
  ///Serial.println("CUR :" + stepper.getCurrent());
  ///Serial.println("TG :" + stepper.getTarget());
  if (ir.decode(& res)) {
    Serial.println(res.value);
    seed = res.value;
    ///Serial.println(seed, HEX);
    delay(100);
    ir.resume();
  }
  if(seed == on_off) //on_off значение типа long с пульта на включение/выключение
  {
    digitalWrite(5, !(digitalRead(5)));///show lamp of hand control
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
