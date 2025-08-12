#ifndef IHARDWAREINTERFACE_H
#define IHARDWAREINTERFACE_H

/**
 * ハードウェア操作を抽象化するインターフェース
 * テスト時にはモック実装を使用し、実機では実際のArduino関数を使用する
 */
class IHardwareInterface {
public:
    virtual ~IHardwareInterface() = default;
    
    // GPIO操作
    virtual void pinMode(int pin, int mode) = 0;
    virtual void digitalWrite(int pin, int value) = 0;
    virtual int digitalRead(int pin) = 0;
    virtual void analogWrite(int pin, int value) = 0;
    virtual int analogRead(int pin) = 0;
    
    // タイマー/PWM制御
    virtual void setTimerRegister(volatile uint8_t* reg, uint8_t value) = 0;
    virtual void setTimer16Register(volatile uint16_t* reg, uint16_t value) = 0;
    virtual uint16_t getTimer16Register(volatile uint16_t* reg) = 0;
    
    // 時間関数
    virtual void delay(unsigned long ms) = 0;
    virtual unsigned long millis() = 0;
};

#endif