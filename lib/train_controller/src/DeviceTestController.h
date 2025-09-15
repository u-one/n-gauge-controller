#ifndef DEVICETESTCONTROLLER_H
#define DEVICETESTCONTROLLER_H

#include "TrainController.h"
#include <ToggleSwitch.h>
#include <PushButtonRotaryEncoder.h>

class DeviceTestController : public TrainController {
public:
    DeviceTestController(
        MotorController* motorController, 
        TwoLinesCharacterDisplay* display, 
        RotarySwitch* rotarySwitch, 
        ToggleSwitch* directionSwitch,
        PushButtonRotaryEncoder* rotaryEncoder
    );
    
    void begin() override;
    void update() override;
    void onRotaryEncoderEvent(RotaryDirection dir);

private:
    RotarySwitch* _rotarySwitch;
    ToggleSwitch* _directionSwitch;
    PushButtonRotaryEncoder* _rotaryEncoder;
    int _rotaryCount = 0;

    void runTestSequence();
    void updateDisplay();
};

#endif