#include "PushButtonRotaryEncoder.h"

PushButtonRotaryEncoder* PushButtonRotaryEncoder::_instance = nullptr;

PushButtonRotaryEncoder::PushButtonRotaryEncoder(int pinCLK, int pinDT, int pinSW) 
    : _pinCLK(pinCLK), _pinDT(pinDT), _pinSW(pinSW), _lastButtonState(true),
      _rotaryHandler(nullptr), _buttonHandler(nullptr) {
    _instance = this;
}

void PushButtonRotaryEncoder::setup() {
    pinMode(_pinCLK, INPUT_PULLUP);
    pinMode(_pinDT, INPUT_PULLUP);
    pinMode(_pinSW, INPUT_PULLUP);
    _lastButtonState = digitalRead(_pinSW);
}

void PushButtonRotaryEncoder::setupInterrupt() {
    attachInterrupt(digitalPinToInterrupt(_pinCLK), interruptHandler, RISING);
}

void PushButtonRotaryEncoder::onRotary(RotaryEventHandler handler) {
    _rotaryHandler = handler;
}

void PushButtonRotaryEncoder::onButton(ButtonEventHandler handler) {
    _buttonHandler = handler;
}

void PushButtonRotaryEncoder::interruptHandler() {
    if (_instance) {
        _instance->handleRotation();
    }
}

void PushButtonRotaryEncoder::handleRotation() {
    if (!_rotaryHandler) return;
    
    int clk = digitalRead(_pinCLK);
    int dat = digitalRead(_pinDT);
    
    if (clk == dat) {
        _rotaryHandler(RotaryDirection::Clockwise);
    } else {
        _rotaryHandler(RotaryDirection::CounterClockwise);
    }
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