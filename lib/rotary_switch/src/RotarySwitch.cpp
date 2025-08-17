#include "RotarySwitch.h"

const RotarySwitch::Threshold RotarySwitch::_thresholds[] = {
    {5, RotarySwitchPosition::State1},      // 0〜10
    {84, RotarySwitchPosition::State2},     // 79〜89
    {167, RotarySwitchPosition::State3},    // 162〜172
    {253, RotarySwitchPosition::State4},    // 248〜258
    {338, RotarySwitchPosition::State5},    // 333〜343
    {425, RotarySwitchPosition::State6},    // 420〜430
    {510, RotarySwitchPosition::State7},    // 505〜515
    {595, RotarySwitchPosition::State8},    // 590〜600
    {680, RotarySwitchPosition::State9},    // 675〜685
    {766, RotarySwitchPosition::State10},   // 761〜771
    {852, RotarySwitchPosition::State11},   // 847〜857
    {938, RotarySwitchPosition::State12}    // 933〜943
};

const int RotarySwitch::_thresholdCount = sizeof(_thresholds) / sizeof(Threshold);

RotarySwitch::RotarySwitch(int analogPin, int tolerance) 
    : _analogPin(analogPin), _tolerance(tolerance), 
      _currentPosition(RotarySwitchPosition::Unknown), 
      _lastRawValue(-1) {
}

void RotarySwitch::setup() {
    pinMode(_analogPin, INPUT);
    // 初期値読み取り
    _lastRawValue = analogRead(_analogPin);
    _currentPosition = valueToPosition(_lastRawValue);
}

RotarySwitchPosition RotarySwitch::read() {
    int rawValue = analogRead(_analogPin);
    _lastRawValue = rawValue;
    
    _currentPosition = valueToPosition(rawValue);
    
    return _currentPosition;
}

RotarySwitchPosition RotarySwitch::getCurrentPosition() {
    return _currentPosition;
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