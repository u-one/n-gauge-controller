#ifndef ROTARYSWITCH_H
#define ROTARYSWITCH_H

#include <Arduino.h>

enum class RotarySwitchPosition {
    Unknown,
    State1,
    State2,
    State3,
    State4,
    State5,
    State6,
    State7,
    State8,
    State9,
    State10,
    State11,
    State12
};

class RotarySwitch {
public:
    RotarySwitch(int analogPin, int tolerance = 5);
    void setup();
    RotarySwitchPosition read();
    RotarySwitchPosition getCurrentPosition();
    bool hasChanged();
    int getRawValue();

private:
    int _analogPin;
    int _tolerance;
    RotarySwitchPosition _currentPosition;
    RotarySwitchPosition _lastPosition;
    int _lastRawValue;
    
    struct Threshold {
        int value;
        RotarySwitchPosition position;
    };
    
    static const Threshold _thresholds[];
    static const int _thresholdCount;
    
    RotarySwitchPosition valueToPosition(int value);
};

#endif