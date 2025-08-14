#include <Arduino.h>
#include "MotorController.h"
#include "LiquidCrystal3.h"
#include "RotarySwitch.h"
#include "SimpleTrainController.h"
#include "TwoLinesCharacterDisplay.h"
#include "DemoTrainController.h"
#include "AutoTrainController.h"
#include "MasconTrainController.h"

const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用
MotorController motorController(IN1, IN2, ENA);

// LiquidCrystal lcd(clock, data, latch);
// d0-d13,a0-a5 select free!
LiquidCrystal lcd(2, 4, 3);
TwoLinesCharacterDisplay display(&lcd);

// ロータリースイッチとトレインコントローラー
RotarySwitch rotarySwitch(A0);
//SimpleTrainController trainController(&motorController, &rotarySwitch, &display);
//AutoTrainController trainController(&motorController, &display);
MasconTrainController trainController(&motorController, &rotarySwitch, &display);

void setup() {
  display.setup();
  motorController.setup();
  rotarySwitch.setup();
  trainController.begin();
}

void loop() {
  trainController.update();
}
