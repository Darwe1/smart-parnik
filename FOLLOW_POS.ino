
#include <GyverStepper.h>
GStepper<STEPPER2WIRE> stepper(2048, 10, 9, 11);
int n = 0;
// мотор с драйвером ULN2003 подключается по порядку пинов, но крайние нужно поменять местами
// то есть у меня подключено D2-IN1, D3-IN2, D4-IN3, D5-IN4, но в программе поменял 5 и 2
void setup() {
  Serial.begin(9600);
  // режим следования к целевй позиции
  stepper.setRunMode(FOLLOW_POS);
  // установка макс. скорости в шагах/сек
  stepper.setMaxSpeed(400);
  // установка ускорения в шагах/сек/сек
  stepper.setAcceleration(500);
}
void loop() {
  // просто крутим туды-сюды
  if (!stepper.tick()) {
    stepper.setTarget(900);
    stepper.disable();
    delay(2000);
    stepper.setCurrent(0);
    stepper.enable();
  }
  
  // график положения
  static uint32_t tmr2;
  if (millis() - tmr2 > 20) {
    tmr2 = millis();
    Serial.println(stepper.getCurrent());
  } 
}
