#ifndef MOCKHARDWARE_H
#define MOCKHARDWARE_H

#include "IHardwareInterface.h"
#include <map>
#include <stdint.h>

/**
 * テスト用のモックハードウェア実装
 * GPIO状態やレジスタ値を内部で管理し、テストで検証可能にする
 */
class MockHardware : public IHardwareInterface {
private:
    std::map<int, int> pinModes;
    std::map<int, int> digitalPins;
    std::map<int, int> analogPins;
    std::map<int, int> analogValues;  // analogRead用の値
    std::map<void*, uint8_t> timerRegisters8;
    std::map<void*, uint16_t> timerRegisters16;
    unsigned long currentTime;
    
public:
    MockHardware() : currentTime(0) {}
    
    void pinMode(int pin, int mode) override {
        pinModes[pin] = mode;
    }
    
    void digitalWrite(int pin, int value) override {
        digitalPins[pin] = value;
    }
    
    int digitalRead(int pin) override {
        return digitalPins.count(pin) ? digitalPins[pin] : 0;
    }
    
    void analogWrite(int pin, int value) override {
        analogPins[pin] = value;
    }
    
    int analogRead(int pin) override {
        return analogValues.count(pin) ? analogValues[pin] : 0;
    }
    
    void setTimerRegister(volatile uint8_t* reg, uint8_t value) override {
        timerRegisters8[const_cast<uint8_t*>(reg)] = value;
    }
    
    void setTimer16Register(volatile uint16_t* reg, uint16_t value) override {
        timerRegisters16[const_cast<uint16_t*>(reg)] = value;
    }
    
    uint16_t getTimer16Register(volatile uint16_t* reg) override {
        return timerRegisters16.count(const_cast<uint16_t*>(reg)) ? 
               timerRegisters16[const_cast<uint16_t*>(reg)] : 0;
    }
    
    void delay(unsigned long ms) override {
        currentTime += ms;
    }
    
    unsigned long millis() override {
        return currentTime;
    }
    
    // テスト用のヘルパーメソッド
    void setAnalogReadValue(int pin, int value) {
        analogValues[pin] = value;
    }
    
    int getDigitalPinValue(int pin) const {
        return digitalPins.count(pin) ? digitalPins.at(pin) : 0;
    }
    
    int getAnalogPinValue(int pin) const {
        return analogPins.count(pin) ? analogPins.at(pin) : 0;
    }
    
    int getPinMode(int pin) const {
        return pinModes.count(pin) ? pinModes.at(pin) : 0;
    }
    
    void setCurrentTime(unsigned long time) {
        currentTime = time;
    }
    
    void reset() {
        pinModes.clear();
        digitalPins.clear();
        analogPins.clear();
        analogValues.clear();
        timerRegisters8.clear();
        timerRegisters16.clear();
        currentTime = 0;
    }
};

#endif