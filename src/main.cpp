#include <Arduino.h>
#include "MotorController.h"
#include "LiquidCrystal3.h"

const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用
MotorController motorController(IN1, IN2, ENA);

// LiquidCrystal lcd(clock, data, latch);
// d0-d13,a0-a5 select free!
LiquidCrystal lcd(2, 4, 3);


int speed = 0;

// put function declarations here:
void go(int);
void pattern1();
void show_lcd();


void setup() {
  lcd.begin(16, 2);
  lcd.print("OK");

  motorController.setup();
}

void loop() {
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
  for(speed=0;speed<max;speed++) {
    motorController.speed(speed);
    show_lcd();
    delay(20);
  }
  
  for(speed=max;speed>=0;speed--) {
    motorController.speed(speed);
    show_lcd();
    delay(20);
  }
}

void show_lcd() {
  lcd.setCursor(0, 1);
  lcd.print("SPEED:");
  if (speed < 10) {
    lcd.print("__");
  } else if (speed < 100) {
    lcd.print("_");
  }
  lcd.print(speed);
}
