#include "TrainController.h"

TrainController::TrainController(MotorController* motorController, TwoLinesCharacterDisplay* display)
    : _motorController(motorController), _display(display) {
    
    // 初期状態
    _currentState = {0, true, "Init", RotarySwitchPosition::Unknown, false};
    _lastState = _currentState;
}