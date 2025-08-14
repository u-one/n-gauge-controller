#include "TrainController.h"
#include <TwoLinesCharacterDisplay.h>

const TrainController::ControlMapping TrainController::_mappings[] = {
    {RotarySwitchPosition::State1,  0,  true,  "Emergency"},  // Emergency brake
    {RotarySwitchPosition::State2,  80, false, "B4"},         // Brake 4
    {RotarySwitchPosition::State3,  60, false, "B3"},         // Brake 3
    {RotarySwitchPosition::State4,  40, false, "B2"},         // Brake 2
    {RotarySwitchPosition::State5,  20, false, "B1"},         // Brake 1
    {RotarySwitchPosition::State6,  0,  true,  "N"},          // Neutral
    {RotarySwitchPosition::State7,  20, true,  "P1"},         // Power 1
    {RotarySwitchPosition::State8,  40, true,  "P2"},         // Power 2
    {RotarySwitchPosition::State9,  60, true,  "P3"},         // Power 3
    {RotarySwitchPosition::State10, 80, true,  "P4"},         // Power 4
    {RotarySwitchPosition::State11, 100,true,  "P5"},         // Power 5
    {RotarySwitchPosition::State12, 120,true,  "P6"},         // Power 6
    {RotarySwitchPosition::Unknown, 0,  true,  "Unknown"}     // Unknown
};

const int TrainController::_mappingCount = sizeof(_mappings) / sizeof(ControlMapping);

TrainController::TrainController(MotorController* motorController, RotarySwitch* rotarySwitch, TwoLinesCharacterDisplay* display)
    : _motorController(motorController), _rotarySwitch(rotarySwitch), _display(display) {

    // 初期状態
    _currentState = {0, true, "Init", RotarySwitchPosition::Unknown, false};
    _lastState = _currentState;
}

void TrainController::begin() {
    // 初期状態を読み取り
    RotarySwitchPosition position = _rotarySwitch->getCurrentPosition();
    _currentState = mapPositionToControl(position);
    _lastState = _currentState;
    _currentState.hasChanged = false;
}

void TrainController::update() {
    // ロータリースイッチの状態を読み取り
    RotarySwitchPosition newPosition = _rotarySwitch->read();
    
    // 前回の状態を保存
    _lastState = _currentState;
    
    // 新しい制御状態にマッピング
    _currentState = mapPositionToControl(newPosition);
    
    // 変更があったかチェック
    _currentState.hasChanged = _rotarySwitch->hasChanged();
    
    // 状態が変更された場合、モーターを制御
    if (_currentState.hasChanged) {
        // 方向設定
        if (_currentState.isForward) {
            _motorController->forward();
        } else {
            _motorController->reverse();
        }
        
        // 速度設定（800Hz固定、duty比で速度制御）
        _motorController->setPwmSettings(800, _currentState.speed);
        
        // 停止の場合
        if (_currentState.speed == 0) {
            _motorController->stop();
        }

        String line1 = _currentState.displayName + " " + String(_currentState.speed) + "%";
        String line2 = String(_motorController->getCurrentFrequency()) + "Hz " + (_currentState.isForward ? "FWD" : "REV");

        _display->setLines(line1, line2);
  }

}

TrainControlState TrainController::getCurrentState() {
    return _currentState;
}

bool TrainController::hasStateChanged() {
    return _currentState.hasChanged;
}

TrainControlState TrainController::mapPositionToControl(RotarySwitchPosition position) {
    // マッピングテーブルから対応する制御パラメータを検索
    for (int i = 0; i < _mappingCount; i++) {
        if (_mappings[i].position == position) {
            return {
                _mappings[i].speed,
                _mappings[i].isForward,
                _mappings[i].displayName,
                position,
                false  // hasChangedはupdate()で設定
            };
        }
    }
    
    // 見つからない場合は安全のため停止
    return {0, true, "Safe Stop", position, false};
}