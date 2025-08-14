#include "RotarySwitch.h"

const RotarySwitch::Threshold RotarySwitch::_thresholds[] = {
    {5, RotarySwitchPosition::State1},      // 0〜10
    {91, RotarySwitchPosition::State2},     // 86〜96
    {185, RotarySwitchPosition::State3},    // 180〜190
    {278, RotarySwitchPosition::State4},    // 273〜283
    {372, RotarySwitchPosition::State5},    // 367〜377
    {465, RotarySwitchPosition::State6},    // 460〜470
    {558, RotarySwitchPosition::State7},    // 553〜563
    {651, RotarySwitchPosition::State8},    // 646〜656
    {743, RotarySwitchPosition::State9},    // 738〜748
    {837, RotarySwitchPosition::State10},   // 832〜842
    {931, RotarySwitchPosition::State11},   // 926〜936
    {1023, RotarySwitchPosition::State12}   // 1018〜1023
};

const int RotarySwitch::_thresholdCount = sizeof(_thresholds) / sizeof(Threshold);

RotarySwitch::RotarySwitch(int analogPin, int tolerance) 
    : _analogPin(analogPin), _tolerance(tolerance), 
      _currentPosition(RotarySwitchPosition::Unknown), 
      _lastPosition(RotarySwitchPosition::Unknown),
      _lastRawValue(-1) {
}

void RotarySwitch::setup() {
    pinMode(_analogPin, INPUT);
    // 初期値読み取り
    _lastRawValue = analogRead(_analogPin);
    _currentPosition = valueToPosition(_lastRawValue);
    _lastPosition = _currentPosition;
}

RotarySwitchPosition RotarySwitch::read() {
    int rawValue = analogRead(_analogPin);
    _lastRawValue = rawValue;
    
    _lastPosition = _currentPosition;
    _currentPosition = valueToPosition(rawValue);
    
    return _currentPosition;
}

RotarySwitchPosition RotarySwitch::getCurrentPosition() {
    return _currentPosition;
}

bool RotarySwitch::hasChanged() {
    return _currentPosition != _lastPosition;
}

int RotarySwitch::getRawValue() {
    return _lastRawValue;
}

RotarySwitchPosition RotarySwitch::valueToPosition(int value) {
    for (int i = 0; i < _thresholdCount; i++) {
        int threshold = _thresholds[i].value;
        if (value >= (threshold - _tolerance) && value <= (threshold + _tolerance)) {
            return _thresholds[i].position;
        }
    }
    
    return RotarySwitchPosition::Unknown;
}