#include "MasconTrainController.h"

MasconTrainController::MasconTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display)
    : TrainController(motorController, display) {
}

void MasconTrainController::begin() {
    // 空の実装
}

void MasconTrainController::update() {
    // 空の実装
}

TrainControlState MasconTrainController::getCurrentState() {
    return _currentState;
}

bool MasconTrainController::hasStateChanged() {
    return _currentState.hasChanged;
}