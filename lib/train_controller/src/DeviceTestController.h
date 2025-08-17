#ifndef DEVICETESTCONTROLLER_H
#define DEVICETESTCONTROLLER_H

#include "TrainController.h"
#include <ToggleSwitch.h>

class DeviceTestController : public TrainController {
public:
    DeviceTestController(
        MotorController* motorController, 
        TwoLinesCharacterDisplay* display, 
        RotarySwitch* rotarySwitch, 
        ToggleSwitch* directionSwitch
    );
    
    void begin() override;
    void update() override;

private:
    RotarySwitch* _rotarySwitch;
    ToggleSwitch* _directionSwitch;

    void runTestSequence();
    void updateDisplay();
};

#endif