#include "MotorController.h"
#include <Arduino.h>

MotorController::MotorController(int in1Pin, int in2Pin, int pwmPin)
    : _pwmPin(pwmPin), _in1Pin(in1Pin), _in2Pin(in2Pin) {
    pinMode(_in1Pin, OUTPUT);
    pinMode(_in2Pin, OUTPUT);
    pinMode(_pwmPin, OUTPUT);
}

// モーターを前進させる（正転）
void MotorController::forward() {
    digitalWrite(_in1Pin, HIGH);
    digitalWrite(_in2Pin, LOW);
}

// モーターを後退させる（反転）
void MotorController::reverse() {
    digitalWrite(_in1Pin, LOW);
    digitalWrite(_in2Pin, HIGH);
}

void MotorController::speed(int speed) {
    analogWrite(_pwmPin, speed);
}

// モーターを停止
void MotorController::stop() {
    digitalWrite(_in1Pin, LOW);
    digitalWrite(_in2Pin, LOW);
    analogWrite(_pwmPin, 0);
}
