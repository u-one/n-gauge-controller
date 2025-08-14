#include "AutoTrainController.h"

AutoTrainController::AutoTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display)
    : TrainController(motorController, display) {
}

void AutoTrainController::begin() {
    // 空の実装
}

void AutoTrainController::update() {
    _motorController->stop();

    _display->setLines("VVVF PWM Demo", "");
    delay(1000);

    _motorController->forward();

    // 正確な周波数テスト
    _motorController->setPwmSettings(800, 40.0);
    String line1 = String(_motorController->getCurrentFrequency()) + "Hz 40%";
    _display->setLines(line1, "Forward");
    delay(2000);

    _motorController->stop();
    delay(2000);

    _motorController->reverse();
    // 中周波数テスト
    _motorController->setPwmSettings(1200, 40.0);
    line1 = String(_motorController->getCurrentFrequency()) + "Hz 40%";
    _display->setLines(line1, "Reverse");
    delay(2000);
}

TrainControlState AutoTrainController::getCurrentState() {
    return _currentState;
}

bool AutoTrainController::hasStateChanged() {
    return _currentState.hasChanged;
}