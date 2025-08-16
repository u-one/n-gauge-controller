#ifndef SIMPLETRAINCONTROLLER_H
#define SIMPLETRAINCONTROLLER_H

#include "TrainController.h"
#include "RotarySwitch.h"

class SimpleTrainController : public TrainController {
public:
    SimpleTrainController(MotorController* motorController, RotarySwitch* rotarySwitch, TwoLinesCharacterDisplay* display);
    
    void begin() override;
    void update() override;

private:
    RotarySwitch* _rotarySwitch;
    
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