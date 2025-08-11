#include "MotorController.h"
#include <Arduino.h>

MotorController::MotorController(int in1Pin, int in2Pin, int pwmPin)
    : _pwmPin(pwmPin), _in1Pin(in1Pin), _in2Pin(in2Pin), _currentFrequency(490), _currentDutyCycle(0.0) {
}

void MotorController::setup() {
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
    _currentDutyCycle = 0.0;
}

// PWM周波数を正確に設定（VVVF音再現用：30-2000Hz）
void MotorController::setPwmFrequency(int frequency) {
    // VVVF音再現のため30-2000Hz範囲に制限
    _currentFrequency = constrain(frequency, 30, 2000);
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // Timer1をPhase-Correct PWMモードに設定（Pin 9, 10）
        // WGM13:WGM12:WGM11:WGM10 = 1010 (Phase-Correct PWM, TOP=ICR1)
        TCCR1A = (1 << COM1B1) | (1 << WGM11);  // Pin 10 (OC1B) 使用
        if (_pwmPin == 9) {
            TCCR1A = (1 << COM1A1) | (1 << WGM11);  // Pin 9 (OC1A) 使用
        }
        
        // CS11 = 8分周, WGM13 = Phase-Correct PWM
        TCCR1B = (1 << WGM13) | (1 << CS11);
        
        // ICR1で周波数を設定: f = 16MHz/(2*8*(ICR1+1))
        // ICR1 = (16000000 / (2 * 8 * frequency)) - 1
        uint16_t icr1_value = (16000000UL / (2UL * 8UL * _currentFrequency)) - 1;
        ICR1 = icr1_value;
        
        // duty比をリセット（現在の値を維持）
        setDutyCycle(_currentDutyCycle);
        
    } else if (_pwmPin == 3 || _pwmPin == 11) {
        _updateTimer2Settings(_currentFrequency);
    } else if (_pwmPin == 5 || _pwmPin == 6) {
        // Timer0 (Pin 5, 6) - 従来の設定を維持
        if (_currentFrequency <= 62) {
            TCCR0B = (TCCR0B & 0xF8) | 0x05; // 62Hz
        } else if (_currentFrequency <= 244) {
            TCCR0B = (TCCR0B & 0xF8) | 0x04; // 244Hz
        } else {
            TCCR0B = (TCCR0B & 0xF8) | 0x04; // 244Hz
        }
    }
}

// Timer2の詳細設定（Pin 3, 11用）- VVVF音再現用（2000Hz対応）
void MotorController::_updateTimer2Settings(int frequency) {
    if (frequency <= 122) {
        TCCR2B = (TCCR2B & 0xF8) | 0x07; // 122Hz
    } else if (frequency <= 244) {
        TCCR2B = (TCCR2B & 0xF8) | 0x06; // 244Hz
    } else if (frequency <= 488) {
        TCCR2B = (TCCR2B & 0xF8) | 0x05; // 488Hz
    } else if (frequency <= 976) {
        TCCR2B = (TCCR2B & 0xF8) | 0x04; // 976Hz
    } else {
        TCCR2B = (TCCR2B & 0xF8) | 0x03; // 3906Hz (2000Hz対応)
    }
}

// duty比を設定（0.0-100.0%）
void MotorController::setDutyCycle(float dutyCycle) {
    _currentDutyCycle = constrain(dutyCycle, 0.0, 100.0);
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // Timer1のPhase-Correct PWMの場合、OCRレジスタで直接制御
        uint16_t ocrValue = (uint32_t)ICR1 * _currentDutyCycle / 100.0;
        
        if (_pwmPin == 10) {
            OCR1B = ocrValue;  // Pin 10 (OC1B)
        } else if (_pwmPin == 9) {
            OCR1A = ocrValue;  // Pin 9 (OC1A)
        }
    } else {
        // 他のピンは従来通りanalogWrite使用
        int pwmValue = map(_currentDutyCycle * 10, 0, 1000, 0, 255);
        analogWrite(_pwmPin, pwmValue);
    }
}

// PWM周波数とduty比を同時に設定
void MotorController::setPwmSettings(int frequency, float dutyCycle) {
    setPwmFrequency(frequency);
    setDutyCycle(dutyCycle);
}

// 現在設定されている周波数を取得
int MotorController::getCurrentFrequency() {
    return _currentFrequency;
}
