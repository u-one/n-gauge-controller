#include <Arduino.h>
#include "MotorController.h"
#include "LiquidCrystal3.h"
#include "RotarySwitch.h"
#include "SimpleTrainController.h"
#include "TwoLinesCharacterDisplay.h"
#include "DemoTrainController.h"
#include "AutoTrainController.h"
#include "MasconTrainController.h"
#include <ToggleSwitch.h>
#include <DeviceTestController.h>

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
const int DIR_SW = A1; // トグルスイッチのピン番号
ToggleSwitch toggleSwitch(DIR_SW); 
//SimpleTrainController trainController(&motorController, &rotarySwitch, &display);
//AutoTrainController trainController(&motorController, &display);
MasconTrainController trainController(&motorController, &display, &rotarySwitch, &toggleSwitch);
//DeviceTestController trainController(&motorController, &display, &rotarySwitch, &toggleSwitch);

void setup() {
  display.setup();
  motorController.setup();
  rotarySwitch.setup();
  toggleSwitch.setup();
  trainController.begin();
}

void loop() {
  trainController.update();
}
