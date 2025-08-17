#include "DeviceTestController.h"

DeviceTestController::DeviceTestController(
    MotorController* motorController, 
    TwoLinesCharacterDisplay* display, 
    RotarySwitch* rotarySwitch, 
    ToggleSwitch* directionSwitch
) : TrainController(motorController, display),
      _rotarySwitch(rotarySwitch),
      _directionSwitch(directionSwitch)
{
}

void DeviceTestController::begin() {
    _display->setLines("Device Test", "Ready");
}

void DeviceTestController::update() {
    _rotarySwitch->read();

    int rotarySwitchValue = _rotarySwitch->getRawValue();
    bool direction = _directionSwitch->read();

    _display->setLine1("Rotary: " + String(rotarySwitchValue));
    _display->setLine2("Dir: " + String(direction ? "FWD" : "REV"));
    delay(20);
}