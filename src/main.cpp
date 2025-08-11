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
void pwmDemo();


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

void pattern1() {
  motorController.stop();

  lcd.clear();
  lcd.print("VVVF PWM Demo");
  delay(1000);
  
  motorController.forward();
  
  // 正確な周波数テスト
  motorController.setPwmSettings(800, 40.0);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz 40%");
  lcd.setCursor(0, 1);
  lcd.print("Forward");
  delay(2000);

  motorController.stop();
  delay(2000);

  motorController.reverse();
  // 中周波数テスト
  motorController.setPwmSettings(1200, 40.0);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz 40%");
  lcd.setCursor(0, 1);
  lcd.print("Reverse");
  delay(2000);

}

void pwmDemo() {
  // VVVF音再現用PWMデモ（最大2000Hz）
  lcd.clear();
  lcd.print("VVVF PWM Demo");
  delay(1000);
  
  motorController.forward();
  
  // 低周波数スタート（モーター起動音）
  motorController.setPwmSettings(30, 20.0);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz 20%");
  lcd.setCursor(0, 1);
  lcd.print("Motor Start");
  delay(2000);
  
  // 中周波数（加速中）
  motorController.setPwmSettings(800, 50.0);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz 50%");
  lcd.setCursor(0, 1);
  lcd.print("Accelerating");
  delay(2000);
  
  // 高周波数（最高速度付近）
  motorController.setPwmSettings(2000, 75.0);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz 75%");
  lcd.setCursor(0, 1);
  lcd.print("Max Speed");
  delay(2000);
  
  // VVVF音シミュレーション（正確な周波数スイープ）
  lcd.clear();
  lcd.print("VVVF Precise");
  
  // 加速パターン（30Hz → 2000Hz）- 実際の設定周波数表示
  for(int f = 30; f <= 2000; f += 100) {
    float duty = map(f, 30, 2000, 20, 80);
    motorController.setPwmSettings(f, duty);
    lcd.setCursor(0, 1);
    lcd.print("     ");  // クリア
    lcd.setCursor(0, 1);
    lcd.print(motorController.getCurrentFrequency());
    lcd.print("Hz ");
    lcd.print((int)duty);
    lcd.print("%");
    delay(150);
  }
  
  delay(1000);
  
  // 減速パターン（2000Hz → 30Hz）- 実際の設定周波数表示
  for(int f = 2000; f >= 30; f -= 80) {
    float duty = map(f, 30, 2000, 20, 80);
    motorController.setPwmSettings(f, duty);
    lcd.setCursor(0, 1);
    lcd.print("     ");  // クリア
    lcd.setCursor(0, 1);
    lcd.print(motorController.getCurrentFrequency());
    lcd.print("Hz ");
    lcd.print((int)duty);
    lcd.print("%");
    delay(100);
  }
  
  motorController.stop();
  lcd.clear();
  lcd.print("VVVF Complete");
  delay(1000);
}
