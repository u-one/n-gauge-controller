#include "DeviceTestController.h"

DeviceTestController::DeviceTestController(
    MotorController* motorController, 
    TwoLinesCharacterDisplay* display, 
    RotarySwitch* rotarySwitch, 
    ToggleSwitch* directionSwitch,
    PushButtonRotaryEncoder* rotaryEncoder
) : TrainController(motorController, display),
      _rotarySwitch(rotarySwitch),
      _directionSwitch(directionSwitch),
      _rotaryEncoder(rotaryEncoder)
{
}

void DeviceTestController::begin() {
    _display->setLines("Device Test", "Ready");
}

void DeviceTestController::update() {
    _rotarySwitch->read();

    int rotarySwitchValue = _rotarySwitch->getRawValue();
    bool direction = _directionSwitch->read();

    //_display->setLine1("RotarySW: " + String(rotarySwitchValue));
    _display->setLine1("Rotary: " + String(_rotaryCount));
    _display->setLine2("Dir: " + String(direction ? "FWD" : "REV"));
    delay(20);
}

void DeviceTestController::onRotaryEncoderEvent(RotaryDirection dir) {
    if (dir == RotaryDirection::Clockwise) {
        _rotaryCount += 1;
    } else {
        _rotaryCount -= 1;
    }
    if (_rotaryCount < 0) _rotaryCount = 0;
}
