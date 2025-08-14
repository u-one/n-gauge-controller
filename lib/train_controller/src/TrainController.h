#ifndef TRAINCONTROLLER_H
#define TRAINCONTROLLER_H

#include <Arduino.h>
#include "MotorController.h"
#include "RotarySwitch.h"
#include "TwoLinesCharacterDisplay.h"

struct TrainControlState {
    int speed;
    bool isForward;
    String displayName;
    RotarySwitchPosition switchPosition;
    bool hasChanged;
};

class TrainController {
public:
    TrainController(MotorController* motorController, TwoLinesCharacterDisplay* display);
    virtual ~TrainController() = default;
    
    virtual void begin() = 0;
    virtual void update() = 0;
    virtual TrainControlState getCurrentState() = 0;
    virtual bool hasStateChanged() = 0;

protected:
    MotorController* _motorController;
    TwoLinesCharacterDisplay* _display;
    TrainControlState _currentState;
    TrainControlState _lastState;
};

#endif