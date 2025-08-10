#include <Arduino.h>
#include "MotorController.h"

const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用
MotorController motorController(IN1, IN2, ENA);

// put function declarations here:
void go(int);
void pattern1();

void setup() {
  motorController.setup();
  //pinMode(IN1, OUTPUT);
  //pinMode(IN2, OUTPUT);
  //pinMode(ENA, OUTPUT);
}

void loop() {
  //digitalWrite(IN1, HIGH);
  //digitalWrite(IN2, LOW);
  //analogWrite(ENA, 128);
  //delay(1000);
   pattern1();
}

void pattern1() {
  motorController.forward();
  go(120);
  delay(2000);
  motorController.reverse();
  go(120);
  delay(2000);
  motorController.stop();

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
