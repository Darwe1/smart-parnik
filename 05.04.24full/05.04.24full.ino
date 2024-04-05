#include <DHT.h>
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
#define dv 43
#define nagrev 44
#define careta 45
#define vent 7
#define pomp 5
#define lamp 6
#define lamp2 46
#define fotores A0
#define vlaznost A2
#define on_off 0xFFA25D
long seed;
int n = 0;
bool mi = 0; //работает ли устройство в ручном режиме
decode_results res;
DHT dht(2, DHT11);  
int a1 = 0;
int a2 = 0;
int a3 = 0;
int a4 = 0;
float t = 0;
///добавить флаг питания двигателя!

void setup()
{
  ///lcd.begin(16,2);
  pinMode(nagrev, OUTPUT);
  pinMode(fotores, INPUT);
  pinMode(vlaznost, INPUT);
  pinMode(lamp, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(careta, OUTPUT);
  pinMode(vent, OUTPUT);
  pinMode(pompa, OUTPUT);
  pinMode(dv, OUTPUT);
  Serial.begin(9600);
  stepper.autoPower(true);
  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(300);
  stepper.setAcceleration(400);  
  stepper.setCurrent(0);
  ir.enableIRIn();
  pinMode(5,OUTPUT);
  dht.begin();         
  lcd.begin(16, 2); // выставляем скорость COM порта
  ///irrecv.enableIRIn(); // запускаем прием
  ///lcd.autoscroll();
  digitalWrite(dv, LOW);
  digitalWrite(lamp, HIGH);
  digitalWrite(lamp2, LOW);
  digitalWrite(vent, HIGH);
  digitalWrite(nagrev, LOW);
  digitalWrite(pomp, HIGH);
}

void lampa(){
  if(analogRead(fotores) <= 500){
    ///Serial.println("lampa 1");
    digitalWrite(lamp2, LOW);
    a1 = 0;
  }
  else
  {
    ///Serial.println("lampa 0");
    digitalWrite(lamp2, HIGH);
    a1 = 1;
  }
}

void pompa(){
    if (analogRead(vlaznost) <= 700) {
    ///Serial.println("pompa 1");
    digitalWrite(pomp, LOW);
    a2 = 1;
    ///сделать так чтобы помпа качала определенное время  
  }
  else{
    ///Serial.println("pompa 0");
    digitalWrite(pomp, HIGH);
    a2 = 0;
  }
}

void caret(){
  if(t > 27.0){
    if(!stepper.tick()){
      stepper.setTarget(400);
    }
    digitalWrite(vent, LOW);
    digitalWrite(nagrev, LOW);
    a3 = 1;
    a4 = 0;
  }
  else if(t <= 27.0 && t >= 24.6){
    if(!stepper.tick()){
      stepper.setTarget(200);
    }
    digitalWrite(vent, HIGH);
    digitalWrite(nagrev, LOW);
    a3 = 0;
    a4 = 0;
  }
  else if(t <= 24.4){
    if(!stepper.tick()){
      stepper.setTarget(0);
    }
    digitalWrite(vent, HIGH);
    digitalWrite(nagrev, HIGH);
    a3 = 0;
    a4 = 1;
    lcd.setCursor(0,2);
    lcd.print("nagrev:");
    lcd.print(a4);
    delay(10000);
    if(!stepper.tick()){
      stepper.setTarget(200);
    }
    a4 = 0;
    a3 = 1;
    lcd.setCursor(0,2);
    lcd.print("nagrev:");
    lcd.print(a4);
    lcd.setCursor(14,1);
    lcd.print(a3);
    digitalWrite(nagrev, LOW);
    digitalWrite(vent, LOW);
    delay(5000);
    digitalWrite(vent, HIGH);
    a3 = 0;
  }
}

void hc(long seed){
  ///if (seed == B1_2 || seed == B1) {
    ////stepper.enable();
  ///}
    if(seed == B2){
      seed = 0;
      if(digitalRead(7) == 0){
        digitalWrite(7, HIGH);
        lcd.clear();
        lcd.print("Vent ne robit");
        lcd.home();
      }
      else
      {
        digitalWrite(7, LOW);
        lcd.clear();
        lcd.print("Vent robit");
        lcd.home();
      }
    }
    if(seed == B3){
      seed = 0;
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
        lcd.print("Pomp ne robit");
        lcd.home();
      }
    }
    if(seed == B4){
      seed = 0;
      if(digitalRead(lamp2) == 0){
        digitalWrite(lamp2, HIGH);
        lcd.clear();
        lcd.print("Lamp robit");
        lcd.home();
      }
      else
      {
        digitalWrite(lamp2, LOW);
        lcd.clear();
        lcd.print("Lamp ne robit");
        lcd.home();
      }
    }
    if(seed == B5){
      seed = 0;
      if(digitalRead(nagrev) == 0){
        digitalWrite(nagrev, HIGH);
        lcd.clear();
        lcd.print("Nagrev robit");
        lcd.home();
      }
      else
      {
        digitalWrite(nagrev, LOW);
        lcd.clear();
        lcd.print("Nagrev ne robit");
        lcd.home();
      }
    }
    if(seed == test){

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

void loop(){
t = dht.readTemperature();
Serial.println(t);
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
    ///Serial.print("ac");
    lcd.print("lampa:");
    lcd.print(a1);
    lcd.print(" vent:");
    lcd.print(a3);
    lcd.setCursor(0,2);
    lcd.print("nagrev:");
    lcd.print(a4);
    lcd.print(" pomp:");
    lcd.print(a2);
    lcd.setCursor(0,0);
    lampa();
    pompa();
    caret();
    ///caret_pomp();
  }
}   
