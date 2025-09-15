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
#include <PushButtonRotaryEncoder.h>

const int IN1 = 8;
const int IN2 = 9;
const int ENA = 10;  // PWM用
MotorController motorController(IN1, IN2, ENA);

// LiquidCrystal lcd(clock, data, latch);
// d0-d13,a0-a5 select free!
LiquidCrystal lcd(A5, A3, A4);
TwoLinesCharacterDisplay display(&lcd);

// ロータリースイッチとトレインコントローラー
RotarySwitch rotarySwitch(A0);
const int DIR_SW = A1; // トグルスイッチのピン番号
ToggleSwitch toggleSwitch(DIR_SW); 
PushButtonRotaryEncoder rotaryEncoder(2, 3, 4); // CLK, DT, SW
//SimpleTrainController trainController(&motorController, &rotarySwitch, &display);
//AutoTrainController trainController(&motorController, &display);
MasconTrainController trainController(&motorController, &display, &rotarySwitch, &toggleSwitch, &rotaryEncoder);
//DeviceTestController trainController(&motorController, &display, &rotarySwitch, &toggleSwitch, &rotaryEncoder);

void rotaryHandler();
void onRotary(RotaryDirection dir);

void setup() {
  Serial.begin(9600);
  display.setup();
  motorController.setup();
  rotarySwitch.setup();
  toggleSwitch.setup();
  rotaryEncoder.setup();
  rotaryEncoder.setupInterrupt(&rotaryHandler);
  rotaryEncoder.onRotary(&onRotary);
  //rotaryEncoder.onRotary(&rotaryHandler);
  trainController.begin();
  Serial.println("Setup complete.");
}

void loop() {
  trainController.update();
}

void rotaryHandler() {
  Serial.println("rotaryHandler");
  rotaryEncoder.handleRotation();
}

void onRotary(RotaryDirection dir) {
  trainController.onRotaryEncoderEvent(dir);
}