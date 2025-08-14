#include "DemoTrainController.h"

DemoTrainController::DemoTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display)
    : TrainController(motorController, display) {
}

void DemoTrainController::begin() {
    // 空の実装
}

void DemoTrainController::update() {
    // VVVF音再現用PWMデモ（最大2000Hz）
    _display->setLines("VVVF PWM Demo", "");
    delay(1000);

    _motorController->forward();

    // 低周波数スタート（モーター起動音）
    _motorController->setPwmSettings(30, 20.0);
    String line1 = String(_motorController->getCurrentFrequency()) + "Hz 20%";
    _display->setLines(line1, "Motor Start");
    delay(2000);

    // 中周波数（加速中）
    _motorController->setPwmSettings(800, 50.0);
    line1 = String(_motorController->getCurrentFrequency()) + "Hz 50%";
    _display->setLines(line1, "Accelerating");
    delay(2000);

    // 高周波数（最高速度付近）
    _motorController->setPwmSettings(2000, 75.0);
    line1 = String(_motorController->getCurrentFrequency()) + "Hz 75%";
    _display->setLines(line1, "Max Speed");
    delay(2000);

    // VVVF音シミュレーション（正確な周波数スイープ）
    _display->setLine1("VVVF Precise");

    // 加速パターン（30Hz → 2000Hz）- 実際の設定周波数表示
    for (int f = 30; f <= 2000; f += 100)
    {
        float duty = map(f, 30, 2000, 20, 80);
        _motorController->setPwmSettings(f, duty);
        String line2 = String(_motorController->getCurrentFrequency()) + "Hz " + String((int)duty) + "%";
        _display->setLine2(line2);
        delay(150);
    }

    delay(1000);

    // 減速パターン（2000Hz → 30Hz）- 実際の設定周波数表示
    for (int f = 2000; f >= 30; f -= 80)
    {
        float duty = map(f, 30, 2000, 20, 80);
        _motorController->setPwmSettings(f, duty);
        String line2 = String(_motorController->getCurrentFrequency()) + "Hz " + String((int)duty) + "%";
        _display->setLine2(line2);
        delay(100);
    }

    _motorController->stop();
    _display->setLines("VVVF Complete", "");
    delay(1000);
}

TrainControlState DemoTrainController::getCurrentState()
{
    return _currentState;
}

bool DemoTrainController::hasStateChanged()
{
    return _currentState.hasChanged;
}