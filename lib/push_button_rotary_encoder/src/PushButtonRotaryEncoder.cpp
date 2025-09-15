#include "PushButtonRotaryEncoder.h"

PushButtonRotaryEncoder::PushButtonRotaryEncoder(int pinCLK, int pinDT, int pinSW) 
    : _pinCLK(pinCLK), _pinDT(pinDT), _pinSW(pinSW), _lastButtonState(true),
      _rotaryHandler(nullptr), _buttonHandler(nullptr) {
}

void PushButtonRotaryEncoder::setup() {
    pinMode(_pinCLK, INPUT_PULLUP);
    pinMode(_pinDT, INPUT_PULLUP);
    pinMode(_pinSW, INPUT_PULLUP);
    _lastButtonState = digitalRead(_pinSW);
}

void PushButtonRotaryEncoder::setupInterrupt(void (*interuptHandler)() ) {
    attachInterrupt(digitalPinToInterrupt(_pinCLK), interuptHandler, RISING);
}

void PushButtonRotaryEncoder::onRotary(RotaryEventHandler handler) {
    _rotaryHandler = handler;
}

void PushButtonRotaryEncoder::onButton(ButtonEventHandler handler) {
    _buttonHandler = handler;
}

void PushButtonRotaryEncoder::handleRotation() {
    if (!_rotaryHandler) return;
    unsigned long currentTime = millis();
    if (currentTime - _lastUpdateTime < 100) {
        return; // デバウンス
    }
    
    int clk = digitalRead(_pinCLK);
    int dat = digitalRead(_pinDT);

    Serial.println("handleRotation" + String(clk) + "," + String(dat));

    if (clk == 0) return; // CLKがHIGHでない場合は無視
    
    if (dat == 1) {
        _rotaryHandler(RotaryDirection::Clockwise);
    } else {
        _rotaryHandler(RotaryDirection::CounterClockwise);
    }
    _lastUpdateTime = currentTime;
}

bool PushButtonRotaryEncoder::isButtonPressed() {
    bool currentState = digitalRead(_pinSW);
    bool pressed = !currentState; // プルアップなので押すとLOW
    
    if (pressed && !_lastButtonState && _buttonHandler) {
        _buttonHandler();
    }
    _lastButtonState = pressed;
    
    return pressed;
}

int PushButtonRotaryEncoder::getRawCLK() {
    return digitalRead(_pinCLK);
}

int PushButtonRotaryEncoder::getRawDT() {
    return digitalRead(_pinDT);
}

int PushButtonRotaryEncoder::getRawSW() {
    return digitalRead(_pinSW);
}