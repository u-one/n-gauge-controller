#include "ToggleSwitch.h"

ToggleSwitch::ToggleSwitch(int pin)
    : _pin(pin), _currentState(false) {
}

void ToggleSwitch::setup() {
    pinMode(_pin, INPUT_PULLUP);
    _currentState = digitalRead(_pin);
}

bool ToggleSwitch::read() {
    _currentState = digitalRead(_pin);
    return _currentState;
}

void ToggleSwitch::onStateChanged() {
    _currentState = digitalRead(_pin);
}
