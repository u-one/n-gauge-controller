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

enum class RotarySwitchStatus {
  Unknown,
  State1,
  State2,
  State3,
  State4,
  State5,
  State6,
  State7,
  State8,
  State9,
  State10,
  State11,
  State12
};

RotarySwitchStatus getRotarySwitchStatus(int value);

void loop() {
  //pwmDemo();
  //pattern1();
  int value = analogRead(A0);  // 0〜1023の範囲で値が返る

  RotarySwitchStatus status = getRotarySwitchStatus(value); 
  bool forward = true;
  switch(status) {
    case RotarySwitchStatus::State2:
      forward = false;
      speed = 80;
      break;
    case RotarySwitchStatus::State3:
      forward = false;
      speed = 60;
      break;
    case RotarySwitchStatus::State4:
      forward = false;
      speed = 40;
      break;
    case RotarySwitchStatus::State5:
      forward = false;
      speed = 20;
      break;
    case RotarySwitchStatus::State6:
      speed = 0;
      break;
    case RotarySwitchStatus::State7:
      speed = 20;
      break;
    case RotarySwitchStatus::State8:
      speed = 40;
      break;
    case RotarySwitchStatus::State9:
      speed = 60;
      break;
    case RotarySwitchStatus::State10:
      speed = 80;
      break;
    default:
      speed = 0; // Unknown状態は停止
  }

  if (forward) {
    motorController.forward();
  } else {
    motorController.reverse();
  }

  // 正確な周波数テスト
  motorController.setPwmSettings(800, speed);
  lcd.clear();
  lcd.print(motorController.getCurrentFrequency());
  lcd.print("Hz ");
  if (forward) {
    lcd.print("Forward");
  } else {
    lcd.print("Reverse"); 
  }

  lcd.setCursor(0, 1);
  lcd.print(speed);

  delay(500);

}

RotarySwitchStatus getRotarySwitchStatus(int value) {
  RotarySwitchStatus status = RotarySwitchStatus::Unknown;
  if (0 <= value && value <= 10) {
    status = RotarySwitchStatus::State1;
  } else if (91 - 5 <= value && value <= 91+5) {
    status = RotarySwitchStatus::State2;
  } else if (185 - 5 <= value && value <= 185+5) {
    status = RotarySwitchStatus::State3;
  } else if (278 - 5 <= value && value <= 278+5) {
    status = RotarySwitchStatus::State4;
  } else if (372 - 5 <= value && value <= 372+5) {
    status = RotarySwitchStatus::State5;
  } else if (465 - 5 <= value && value <= 465+5) {
    status = RotarySwitchStatus::State6;
  } else if (558 - 5 <= value && value <= 558+5) {
    status = RotarySwitchStatus::State7;
  } else if (651 - 5 <= value && value <= 651+5) {
    status = RotarySwitchStatus::State8;
  } else if (743 - 5 <= value && value <= 743+5) {
    status = RotarySwitchStatus::State9;
  } else if (837 - 5 <= value && value <= 837+5) {
    status = RotarySwitchStatus::State10;
  } else if (931 - 5 <= value && value <= 931+5) {
    status = RotarySwitchStatus::State11;
  } else if (1023 - 5 <= value && value <= 1023) {
    status = RotarySwitchStatus::State12;
  } else {
    status = RotarySwitchStatus::Unknown;
  }
  return status;
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
