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
#define nagrev 44
#define careta 45
#define vent 7
#define pomp 5
#define lamp 6
#define lamp2 42
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
  pinMode(fotores, INPUT);
  pinMode(vlaznost, INPUT);
  pinMode(lamp, OUTPUT);
  pinMode(careta, OUTPUT);
  pinMode(vent, OUTPUT);
  pinMode(pompa, OUTPUT);
  Serial.begin(9600);
  stepper.setRunMode(FOLLOW_POS);
  stepper.setSpeed(100);
  ir.enableIRIn();
  pinMode(5,OUTPUT);
  dht.begin();         
  lcd.begin(16, 2); // выставляем скорость COM порта
  ///irrecv.enableIRIn(); // запускаем прием
  ///lcd.autoscroll();
  digitalWrite(lamp, HIGH);
  digitalWrite(lamp2, HIGH);
  digitalWrite(vent, LOW);
  digitalWrite(nagrev, LOW);
  digitalWrite(pomp, LOW);
}

void lampa(){
  lcd.clear();
  if(analogRead(fotores) <= 500){
    ///Serial.println("lampa 1");
    digitalWrite(lamp2, HIGH);
    a1 = 0;
  }
  else
  {
    ///Serial.println("lampa 0");
    digitalWrite(lamp2, LOW);
    a1 = 1;
  }
}

void pompa(){
  lcd.clear();
    if (analogRead(vlaznost) <= 750) {
    ///Serial.println("pompa 1");
    digitalWrite(pompa, HIGH);
    a2 = 1;
    ///сделать так чтобы помпа качала определенное время  
  }
  if (analogRead(vlaznost) > 750) {
    ///Serial.println("pompa 0");
    digitalWrite(pompa, LOW);
    a2 = 0;
  }
}

void caret(){
  lcd.clear();
  if(t > 25.5){
    digitalWrite(vent, HIGH);
    digitalWrite(nagrev, LOW);
    a3 = 1;
    a4 = 0;
    delay(5000);
  }
  else if(t <= 25.5 && t >= 24.6){
    digitalWrite(vent, LOW);
    digitalWrite(nagrev, LOW);
    a3 = 0;
    a4 = 0;
  }
  else if(t <= 24.4){
    digitalWrite(vent, LOW);
    digitalWrite(nagrev, HIGH);
    a3 = 0;
    a4 = 1;
    delay(10000);
    digitalWrite(nagrev, LOW);
    digitalWrite(vent, HIGH);
    delay(5000);
    digitalWrite(vent, LOW);
  }
}

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
t = dht.readTemperature();
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
    lcd.home();
    lampa();
    ///pompa();
    caret();
    ///caret_pomp();
  }
}   
