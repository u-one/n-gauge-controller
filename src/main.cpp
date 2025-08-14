#include <Arduino.h>
#include "MotorController.h"
#include "LiquidCrystal3.h"
#include "RotarySwitch.h"
#include "TrainController.h"
#include "TwoLinesCharacterDisplay.h"

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
TrainController trainController(&motorController, &rotarySwitch, &display);

int speed = 0;

// put function declarations here:
void go(int);
void pattern1();
void show_lcd();
void pwmDemo();


void setup() {
  display.setup();
  motorController.setup();
  rotarySwitch.setup();
  trainController.begin();
}

void loop() {
  // トレインコントローラーを更新
  trainController.update();
  
  
  delay(500);
}


void pattern1() {
  motorController.stop();

  display.setLines("VVVF PWM Demo", "");
  delay(1000);
  
  motorController.forward();
  
  // 正確な周波数テスト
  motorController.setPwmSettings(800, 40.0);
  String line1 = String(motorController.getCurrentFrequency()) + "Hz 40%";
  display.setLines(line1, "Forward");
  delay(2000);

  motorController.stop();
  delay(2000);

  motorController.reverse();
  // 中周波数テスト
  motorController.setPwmSettings(1200, 40.0);
  line1 = String(motorController.getCurrentFrequency()) + "Hz 40%";
  display.setLines(line1, "Reverse");
  delay(2000);

}

void pwmDemo() {
  // VVVF音再現用PWMデモ（最大2000Hz）
  display.setLines("VVVF PWM Demo", "");
  delay(1000);
  
  motorController.forward();
  
  // 低周波数スタート（モーター起動音）
  motorController.setPwmSettings(30, 20.0);
  String line1 = String(motorController.getCurrentFrequency()) + "Hz 20%";
  display.setLines(line1, "Motor Start");
  delay(2000);
  
  // 中周波数（加速中）
  motorController.setPwmSettings(800, 50.0);
  line1 = String(motorController.getCurrentFrequency()) + "Hz 50%";
  display.setLines(line1, "Accelerating");
  delay(2000);
  
  // 高周波数（最高速度付近）
  motorController.setPwmSettings(2000, 75.0);
  line1 = String(motorController.getCurrentFrequency()) + "Hz 75%";
  display.setLines(line1, "Max Speed");
  delay(2000);
  
  // VVVF音シミュレーション（正確な周波数スイープ）
  display.setLine1("VVVF Precise");
  
  // 加速パターン（30Hz → 2000Hz）- 実際の設定周波数表示
  for(int f = 30; f <= 2000; f += 100) {
    float duty = map(f, 30, 2000, 20, 80);
    motorController.setPwmSettings(f, duty);
    String line2 = String(motorController.getCurrentFrequency()) + "Hz " + String((int)duty) + "%";
    display.setLine2(line2);
    delay(150);
  }
  
  delay(1000);
  
  // 減速パターン（2000Hz → 30Hz）- 実際の設定周波数表示
  for(int f = 2000; f >= 30; f -= 80) {
    float duty = map(f, 30, 2000, 20, 80);
    motorController.setPwmSettings(f, duty);
    String line2 = String(motorController.getCurrentFrequency()) + "Hz " + String((int)duty) + "%";
    display.setLine2(line2);
    delay(100);
  }
  
  motorController.stop();
  display.setLines("VVVF Complete", "");
  delay(1000);
}
