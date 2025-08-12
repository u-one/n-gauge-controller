#ifndef TESTABLEMOTORCONTROLLER_H
#define TESTABLEMOTORCONTROLLER_H

#include "IHardwareInterface.h"

/**
 * テスト可能なモーターコントローラ
 * ハードウェア抽象化層を使用して依存性注入を実現
 */
class TestableMotorController {
public:
    TestableMotorController(int in1Pin, int in2Pin, int pwmPin, IHardwareInterface* hardware);
    void setup();
    void forward();
    void reverse();
    void speed(int speed);
    void stop();
    void setPwmFrequency(int frequency);
    void setDutyCycle(float dutyCycle);
    void setPwmSettings(int frequency, float dutyCycle);
    int getCurrentFrequency();
    float getCurrentDutyCycle();
    
private:
    int _in1Pin;
    int _in2Pin;
    int _pwmPin;
    int _currentFrequency;
    float _currentDutyCycle;
    IHardwareInterface* _hardware;
    
    void _updateTimer2Settings(int frequency);
    
    // Arduino固有のレジスタアドレスを模擬
    // 実際のテストでは使用しないが、コンパイルエラーを避けるため定義
    static volatile uint8_t* TCCR0B_ADDR;
    static volatile uint8_t* TCCR1A_ADDR;
    static volatile uint8_t* TCCR1B_ADDR;
    static volatile uint8_t* TCCR2B_ADDR;
    static volatile uint16_t* ICR1_ADDR;
    static volatile uint16_t* OCR1A_ADDR;
    static volatile uint16_t* OCR1B_ADDR;
};

#endif