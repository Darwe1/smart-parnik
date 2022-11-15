
#include <GyverStepper.h>
GStepper<STEPPER2WIRE> stepper(2048, 12, 11, 13);

void setup() {
  Serial.begin(9600);
  stepper.setRunMode(FOLLOW_POS);
  stepper.setTarget(2048, ABSOLUTE);
  stepper.setSpeed(100);
}
void loop() {
   stepper.tick();
}
