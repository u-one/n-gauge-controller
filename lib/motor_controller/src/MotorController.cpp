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
    // 30-2000Hz範囲に制限
    _currentFrequency = constrain(frequency, 30, 40000);
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // TCCR1A: Timer/Counter1 Control Register A
        //  - Arduino(AVRマイコン)のタイマー1の動作モードや出力ピンの動作を設定するためのレジスタ
        // COM1B1: Compare Output Mode for Channel B の 1bit目
        //  - これをセットするとPin10(OC1B)がPWM出力モードになる
        //  - タイマーの値がOCR1Bと一致したときに出力ピン(Pin 10)の状態を変えるようになる
        // WGM11: Waveform Generation Mode の1bit目
        //  - セットするとタイマー1をPhase-Correct PWMモードにする設定の一部になる
        //  - WGM13(TCCR1B側)と組み合わせて、PWMの波形生成モードを決定する

        // Timer1をPhase-Correct PWMモードに設定（Pin 9, 10）
        // WGM13:WGM12:WGM11:WGM10 = 1010 (Phase-Correct PWM, TOP=ICR1)
        TCCR1A = (1 << COM1B1) | (1 << WGM11);  // Pin 10 (OC1B) 使用
        if (_pwmPin == 9) {
            TCCR1A = (1 << COM1A1) | (1 << WGM11);  // Pin 9 (OC1A) 使用
        }
        
        // TCCR1B: Timer/Counter1 Control Register B
        //  - タイマー1の動作モードやクロック分周（タイマーの動く速さ）を設定するレジスタ
        // WGM13: Waveform Generation Mode の4ビット目
        //  - これをセットすることで、タイマー1をPhase-Correct PWM（TOP=ICR1）モードにする設定の一部になる
        //  - WGM11（TCCR1A側）と組み合わせて、PWMの波形生成モードを決定
        // CS11: Clock Select の2bit目
        //  - これをセットすると、タイマー1のクロックが8分周になる
        //  - つまり、タイマー1は16MHzのクロックを8で割った2MHzで動作
        //  - 分周することで、PWMの周波数を細かく調整できる

        //WGM13 = Phase-Correct PWM, CS11 = 8分周
        TCCR1B = (1 << WGM13) | (1 << CS11);
        
        // Arduino UNOのクロック周波数は16MHz
        // 分周比は8

        // Phase-Correct PWMモードでは、タイマーは
        // 0 → TOP（ICR1）までカウントアップ → 0までカウントダウン
        // という動きを繰り返す
        // 通常のFast PWMは「0→TOPまでカウントアップして0に戻る」だけ
        // Phase-Correct PWMは「0→TOP→0」と往復する
        // 周期（1サイクル）の長さ
        // タイマーが0からTOPまでカウントアップするのにかかる時間
        // さらにTOPから0までカウントダウンするのにかかる時間
        // この両方を合わせた時間が1周期（1PWMサイクル）になる

        // ICR1で周波数を設定: f = 16MHz/(2*8*(ICR1+1))
        // ICR1 = (16000000 / (2 * 8 * frequency)) - 1
        uint16_t icr1_value = (16000000UL / (2UL * 8UL * _currentFrequency)) - 1;
        // ICR1: PWMの周期(TOP値)を決めるレジスタ
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

// Timer2の詳細設定（Pin 3, 11用）
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
    _currentDutyCycle = constrain(dutyCycle, 0.0, 1000.0);
    
    if (_pwmPin == 9 || _pwmPin == 10) {
        // Timer1のPhase-Correct PWMの場合、OCRレジスタで直接制御
        // ICR1（TOP値）に対して、duty比の割合だけONになるようOCRレジスタ値を計算
        uint16_t ocrValue = (uint32_t)ICR1 * _currentDutyCycle / 1000.0;
        
        if (_pwmPin == 10) {
            // OCR1B: Output Compare Register 1 B
            // Timer1のBチャンネル（Pin 10, OC1B）のための比較値を保持
            OCR1B = ocrValue;  // Pin 10 (OC1B)
        } else if (_pwmPin == 9) {
            OCR1A = ocrValue;  // Pin 9 (OC1A)
        }
    } else {
        // 他のピンは従来通りanalogWrite使用
        int pwmValue = map(_currentDutyCycle * 100, 0, 1000, 0, 255);
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
