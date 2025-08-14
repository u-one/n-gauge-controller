#include "MasconTrainController.h"

const MasconTrainController::MasconMapping MasconTrainController::_mappings[] = {
    {RotarySwitchPosition::State1,  -100,   "Emergency", true},   // 非常ブレーキ
    {RotarySwitchPosition::State2,  -8,   "B4", true},          // ブレーキ8
    {RotarySwitchPosition::State3,  -4,  "B3", true},          // ブレーキ6
    {RotarySwitchPosition::State4,  -2,  "B2", true},          // ブレーキ4
    {RotarySwitchPosition::State5,  -1,  "B1", true},          // ブレーキ2
    {RotarySwitchPosition::State6,  0,  "N", false},          // 中立
    {RotarySwitchPosition::State7,  1,  "P1", false},         // 力行1
    {RotarySwitchPosition::State8,  2,  "P2", false},         // 力行2
    {RotarySwitchPosition::State9,  4,  "P3", false},         // 力行3
    {RotarySwitchPosition::State10, 8,  "P4", false},         // 力行4
    {RotarySwitchPosition::State11, 16, "P5", false},         // 力行5
    {RotarySwitchPosition::State12, 32, "P6", false},        // 最高出力
    {RotarySwitchPosition::Unknown, 0,   "Unknown", true}      // 不明
};

const int MasconTrainController::_mappingCount = sizeof(_mappings) / sizeof(MasconMapping);

MasconTrainController::MasconTrainController(MotorController* motorController, RotarySwitch* rotarySwitch, TwoLinesCharacterDisplay* display)
    : TrainController(motorController, display),
      _rotarySwitch(rotarySwitch),
      _lastUpdateTime(0),
      _currentSpeed(0),
      _acceleratonRate(0),
      _isAccelerating(false),
      _isDecelerating(false) {
}

void MasconTrainController::begin() {
    _lastUpdateTime = millis();
    
    // 初期状態を読み取り
    RotarySwitchPosition position = _rotarySwitch->getCurrentPosition();
    MasconMapping control = mapPositionToControl(position);
    
    _currentState.speed = 0;
    _currentState.isForward = true;
    _currentState.displayName = control.displayName;
    _currentState.switchPosition = position;
    _currentState.hasChanged = true;
    
    _acceleratonRate = control.acceleratonRate;
    _currentSpeed = 0;
    
    _display->setLines("Mascon Mode", "Ready");
    delay(1000);
}

void MasconTrainController::update() {
    unsigned long currentTime = millis();
    
    // 更新間隔チェック
    if (currentTime - _lastUpdateTime < UPDATE_INTERVAL) {
        return;
    }
    
    // ロータリースイッチの状態を読み取り
    RotarySwitchPosition newPosition = _rotarySwitch->read();
    
    // 前回の状態を保存
    _lastState = _currentState;
    
    // 新しいマスコン制御状態にマッピング
    MasconMapping control = mapPositionToControl(newPosition);
    _acceleratonRate = control.acceleratonRate;
    
    // 変更があったかチェック
    bool positionChanged = _rotarySwitch->hasChanged();
    
    // 速度制御の更新（マスコン風の滑らかな制御）
    updateSpeedControl();
    
    // モーター制御の適用
    applyMotorControl();
    
    // 表示の更新（位置が変わった場合、または速度が変化中）
    if (positionChanged || _isAccelerating || _isDecelerating) {
        String line1 = control.displayName;
        String line2 = "Speed: " + String(_currentSpeed) + "%";
        if (_isAccelerating) {
            line2 += " ACC";
        } else if (_isDecelerating) {
            line2 += " BRK";
        }
        
        _display->setLines(line1, line2);
    }
    
    // 状態更新
    _currentState.speed = _currentSpeed;
    _currentState.isForward = true; // 一方向制御
    _currentState.displayName = control.displayName;
    _currentState.switchPosition = newPosition;
    _currentState.hasChanged = positionChanged;
    
    _lastUpdateTime = currentTime;
    delay(20);
}

void MasconTrainController::updateSpeedControl() {
    // 現在速度を目標速度に向けて調整（マスコン風の滑らかな制御）
    if (_acceleratonRate > 0) {
        // 加速
        _currentSpeed += _acceleratonRate;
        _isAccelerating = true;
        _isDecelerating = false;
    } else if (_acceleratonRate < 0) {
        // 減速・ブレーキ
        _currentSpeed += _acceleratonRate;
        _isAccelerating = false;
        _isDecelerating = true;
    } else {
        // 定速
        _isAccelerating = false;
        _isDecelerating = false;
    }
    
    // 速度範囲の制限
    if (_currentSpeed < 0) _currentSpeed = 0;
    if (_currentSpeed > MAX_SPEED) _currentSpeed = MAX_SPEED;
}

void MasconTrainController::applyMotorControl() {
    if (_currentSpeed == 0) {
        _motorController->stop();
        return;
    }
    
    // 方向設定（一方向制御）
    _motorController->forward();
    
    // 周波数制御
    //int frequency = map(_currentSpeed, 0, MAX_SPEED, 120, 1800);
    int frequency = 180; // EF66, ブロアー音
    //int frequency = 330; // 千代田6000系チョッパ
    //int frequency = 10000; // 無音
    float duty = map(_currentSpeed, 0, MAX_SPEED, 0, 1000);
    
    _motorController->setPwmSettings(frequency, duty);
}

int MasconTrainController::getTargetSpeedForPosition(RotarySwitchPosition position) {
    MasconMapping control = mapPositionToControl(position);
    return control.acceleratonRate;
}

String MasconTrainController::getPositionName(RotarySwitchPosition position) {
    MasconMapping control = mapPositionToControl(position);
    return control.displayName;
}

bool MasconTrainController::isBrakingPosition(RotarySwitchPosition position) {
    MasconMapping control = mapPositionToControl(position);
    return control.isBraking;
}

MasconTrainController::MasconMapping MasconTrainController::mapPositionToControl(RotarySwitchPosition position) {
    // マッピングテーブルから対応するマスコン制御パラメータを検索
    for (int i = 0; i < _mappingCount; i++) {
        if (_mappings[i].position == position) {
            return _mappings[i];
        }
    }
    
    // 見つからない場合は安全のため緊急停止
    return {RotarySwitchPosition::Unknown, 0, "Emergency", true};
}

TrainControlState MasconTrainController::getCurrentState() {
    return _currentState;
}

bool MasconTrainController::hasStateChanged() {
    return _currentState.hasChanged;
}