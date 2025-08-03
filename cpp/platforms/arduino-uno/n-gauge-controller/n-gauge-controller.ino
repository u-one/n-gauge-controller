#include "MotorController.h"

const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用
MotorController motorController(IN1, IN2, ENA);

void setup() {
}


void go(int max) {
  int i;
  for(i=0;i<max;i++) {
    motorController.speed(i);
    delay(20);
  }
  
  for(i=max;i>=0;i--) {
    motorController.speed(i);
    delay(20);
  }

}

void loop() {
  motorController.forward();
  go(120);
  delay(2000);
  motorController.reverse();
  go(120);
  delay(2000);
  motorController.stop();
}
