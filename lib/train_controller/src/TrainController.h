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
    TrainController(MotorController* motorController, RotarySwitch* rotarySwitch, TwoLinesCharacterDisplay* display);
    void begin();
    void update();
    TrainControlState getCurrentState();
    bool hasStateChanged();

private:
    MotorController* _motorController;
    RotarySwitch* _rotarySwitch;
    TwoLinesCharacterDisplay* _display;
    TrainControlState _currentState;
    TrainControlState _lastState;
    
    struct ControlMapping {
        RotarySwitchPosition position;
        int speed;
        bool isForward;
        String displayName;
    };
    
    static const ControlMapping _mappings[];
    static const int _mappingCount;
    
    TrainControlState mapPositionToControl(RotarySwitchPosition position);
};

#endif