#ifndef ARDUINOHARDWARE_H
#define ARDUINOHARDWARE_H

#include "IHardwareInterface.h"
#include <Arduino.h>

/**
 * 実際のArduinoハードウェアに対する操作を行う実装クラス
 */
class ArduinoHardware : public IHardwareInterface {
public:
    void pinMode(int pin, int mode) override {
        ::pinMode(pin, mode);
    }
    
    void digitalWrite(int pin, int value) override {
        ::digitalWrite(pin, value);
    }
    
    int digitalRead(int pin) override {
        return ::digitalRead(pin);
    }
    
    void analogWrite(int pin, int value) override {
        ::analogWrite(pin, value);
    }
    
    int analogRead(int pin) override {
        return ::analogRead(pin);
    }
    
    void setTimerRegister(volatile uint8_t* reg, uint8_t value) override {
        *reg = value;
    }
    
    void setTimer16Register(volatile uint16_t* reg, uint16_t value) override {
        *reg = value;
    }
    
    uint16_t getTimer16Register(volatile uint16_t* reg) override {
        return *reg;
    }
    
    void delay(unsigned long ms) override {
        ::delay(ms);
    }
    
    unsigned long millis() override {
        return ::millis();
    }
};

#endif