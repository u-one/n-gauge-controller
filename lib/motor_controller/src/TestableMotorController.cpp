#include "TestableMotorController.h"

#ifndef UNIT_TEST
// Arduino環境の場合は実際のレジスタを使用
#include <Arduino.h>
volatile uint8_t* TestableMotorController::TCCR0B_ADDR = &TCCR0B;
volatile uint8_t* TestableMotorController::TCCR1A_ADDR = &TCCR1A;
volatile uint8_t* TestableMotorController::TCCR1B_ADDR = &TCCR1B;
volatile uint8_t* TestableMotorController::TCCR2B_ADDR = &TCCR2B;
volatile uint16_t* TestableMotorController::ICR1_ADDR = &ICR1;
volatile uint16_t* TestableMotorController::OCR1A_ADDR = &OCR1A;
volatile uint16_t* TestableMotorController::OCR1B_ADDR = &OCR1B;
#else
// テスト環境の場合はダミーのレジスタを使用
static volatile uint8_t dummy_8bit_reg = 0;
static volatile uint16_t dummy_16bit_reg = 0;
volatile uint8_t* TestableMotorController::TCCR0B_ADDR = &dummy_8bit_reg;
volatile uint8_t* TestableMotorController::TCCR1A_ADDR = &dummy_8bit_reg;
volatile uint8_t* TestableMotorController::TCCR1B_ADDR = &dummy_8bit_reg;
volatile uint8_t* TestableMotorController::TCCR2B_ADDR = &dummy_8bit_reg;
volatile uint16_t* TestableMotorController::ICR1_ADDR = &dummy_16bit_reg;
volatile uint16_t* TestableMotorController::OCR1A_ADDR = &dummy_16bit_reg;
volatile uint16_t* TestableMotorController::OCR1B_ADDR = &dummy_16bit_reg;
#endif

TestableMotorController::TestableMotorController(int in1Pin, int in2Pin, int pwmPin, IHardwareInterface* hardware)
    : _pwmPin(pwmPin), _in1Pin(in1Pin), _in2Pin(in2Pin), _currentFrequency(490), _currentDutyCycle(0.0), _hardware(hardware) {
}

void TestableMotorController::setup() {
    _hardware->pinMode(_in1Pin, 1);  // OUTPUT
    _hardware->pinMode(_in2Pin, 1);  // OUTPUT
    _hardware->pinMode(_pwmPin, 1);  // OUTPUT
}

// モーターを前進させる（正転）
void TestableMotorController::forward() {
    _hardware->digitalWrite(_in1Pin, 1);  // HIGH
    _hardware->digitalWrite(_in2Pin, 0);  // LOW
}

// モーターを後退させる（反転）
void TestableMotorController::reverse() {
    _hardware->digitalWrite(_in1Pin, 0);  // LOW
    _hardware->digitalWrite(_in2Pin, 1);  // HIGH
}

void TestableMotorController::speed(int speed) {
    _hardware->analogWrite(_pwmPin, speed);
}

// モーターを停止
void TestableMotorController::stop() {
    _hardware->digitalWrite(_in1Pin, 0);  // LOW
    _hardware->digitalWrite(_in2Pin, 0);  // LOW
    _hardware->analogWrite(_pwmPin, 0);
    _currentDutyCycle = 0.0;
}

// PWM周波数を正確に設定（VVVF音再現用：30-2000Hz）
void TestableMotorController::setPwmFrequency(int frequency) {
    // VVVF音再現のため30-2000Hz範囲に制限
    _currentFrequency = (frequency < 30) ? 30 : (frequency > 2000) ? 2000 : frequency;
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // Timer1をPhase-Correct PWMモードに設定（Pin 9, 10）
        uint8_t tccr1a_val = (1 << 1) | (1 << 1);  // COM1B1, WGM11の模擬
        if (_pwmPin == 9) {
            tccr1a_val = (1 << 7) | (1 << 1);  // COM1A1, WGM11の模擬
        }
        _hardware->setTimerRegister(TCCR1A_ADDR, tccr1a_val);
        
        // CS11 = 8分周, WGM13 = Phase-Correct PWM
        uint8_t tccr1b_val = (1 << 4) | (1 << 1);  // WGM13, CS11の模擬
        _hardware->setTimerRegister(TCCR1B_ADDR, tccr1b_val);
        
        // ICR1で周波数を設定: f = 16MHz/(2*8*(ICR1+1))
        uint16_t icr1_value = (16000000UL / (2UL * 8UL * _currentFrequency)) - 1;
        _hardware->setTimer16Register(ICR1_ADDR, icr1_value);
        
        // duty比をリセット（現在の値を維持）
        setDutyCycle(_currentDutyCycle);
        
    } else if (_pwmPin == 3 || _pwmPin == 11) {
        _updateTimer2Settings(_currentFrequency);
    } else if (_pwmPin == 5 || _pwmPin == 6) {
        // Timer0 (Pin 5, 6) - 従来の設定を維持
        uint8_t tccr0b_base = 0xF8;  // 下位3bitをクリア
        if (_currentFrequency <= 62) {
            _hardware->setTimerRegister(TCCR0B_ADDR, tccr0b_base | 0x05); // 62Hz
        } else if (_currentFrequency <= 244) {
            _hardware->setTimerRegister(TCCR0B_ADDR, tccr0b_base | 0x04); // 244Hz
        } else {
            _hardware->setTimerRegister(TCCR0B_ADDR, tccr0b_base | 0x04); // 244Hz
        }
    }
}

// Timer2の詳細設定（Pin 3, 11用）- VVVF音再現用（2000Hz対応）
void TestableMotorController::_updateTimer2Settings(int frequency) {
    uint8_t tccr2b_base = 0xF8;  // 下位3bitをクリア
    if (frequency <= 122) {
        _hardware->setTimerRegister(TCCR2B_ADDR, tccr2b_base | 0x07); // 122Hz
    } else if (frequency <= 244) {
        _hardware->setTimerRegister(TCCR2B_ADDR, tccr2b_base | 0x06); // 244Hz
    } else if (frequency <= 488) {
        _hardware->setTimerRegister(TCCR2B_ADDR, tccr2b_base | 0x05); // 488Hz
    } else if (frequency <= 976) {
        _hardware->setTimerRegister(TCCR2B_ADDR, tccr2b_base | 0x04); // 976Hz
    } else {
        _hardware->setTimerRegister(TCCR2B_ADDR, tccr2b_base | 0x03); // 3906Hz (2000Hz対応)
    }
}

// duty比を設定（0.0-100.0%）
void TestableMotorController::setDutyCycle(float dutyCycle) {
    _currentDutyCycle = (dutyCycle < 0.0) ? 0.0 : (dutyCycle > 100.0) ? 100.0 : dutyCycle;
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // Timer1のPhase-Correct PWMの場合、OCRレジスタで直接制御
        uint16_t icr1_value = _hardware->getTimer16Register(ICR1_ADDR);
        uint16_t ocrValue = (uint32_t)icr1_value * _currentDutyCycle / 100.0;
        
        if (_pwmPin == 10) {
            _hardware->setTimer16Register(OCR1B_ADDR, ocrValue);  // Pin 10 (OC1B)
        } else if (_pwmPin == 9) {
            _hardware->setTimer16Register(OCR1A_ADDR, ocrValue);  // Pin 9 (OC1A)
        }
    } else {
        // 他のピンは従来通りanalogWrite使用
        int pwmValue = _currentDutyCycle * 255 / 100;  // 0-255にマップ
        if (pwmValue > 255) pwmValue = 255;
        _hardware->analogWrite(_pwmPin, pwmValue);
    }
}

// PWM周波数とduty比を同時に設定
void TestableMotorController::setPwmSettings(int frequency, float dutyCycle) {
    setPwmFrequency(frequency);
    setDutyCycle(dutyCycle);
}

// 現在設定されている周波数を取得
int TestableMotorController::getCurrentFrequency() {
    return _currentFrequency;
}

// 現在設定されているduty比を取得
float TestableMotorController::getCurrentDutyCycle() {
    return _currentDutyCycle;
}