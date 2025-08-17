#ifndef TOGGLESWITCH_H
#define TOGGLESWITCH_H

#include <Arduino.h>

class ToggleSwitch {
public:
    ToggleSwitch(int pin);
    
    void setup();
    bool read();
    void onStateChanged();

private:
    int _pin;
    bool _currentState;
};

#endif