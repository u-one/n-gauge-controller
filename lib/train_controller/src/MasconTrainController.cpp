#include "MasconTrainController.h"

const MasconTrainController::MasconMapping MasconTrainController::_mappings[] = {
    {RotarySwitchPosition::State1,  16,  "P5", true},         // 力行5
    {RotarySwitchPosition::State2,  8,   "P4", true},         // 力行4
    {RotarySwitchPosition::State3,  4,   "P3", true},         // 力行3
    {RotarySwitchPosition::State4,  2,   "P2", true},         // 力行2
    {RotarySwitchPosition::State5,  1,   "P1", true},         // 力行1
    {RotarySwitchPosition::State6,  0,   "N ", false},        // 中立
    {RotarySwitchPosition::State7,  -1,  "B1", false},        // ブレーキ1
    {RotarySwitchPosition::State8,  -2,  "B2", false},        // ブレーキ2
    {RotarySwitchPosition::State9,  -4,  "B3", false},        // ブレーキ3
    {RotarySwitchPosition::State10, -8,  "B4", false},        // ブレーキ4
    {RotarySwitchPosition::State11, -16, "B5", false},        // ブレーキ5
    {RotarySwitchPosition::State12, -1000, "EB", true}, // 非常ブレーキ
    {RotarySwitchPosition::Unknown, 0,   "Unknown", true}     // 不明
};

const int MasconTrainController::_mappingCount = sizeof(_mappings) / sizeof(MasconMapping);

MasconTrainController::MasconTrainController(
    MotorController* motorController, 
    TwoLinesCharacterDisplay* display, 
    RotarySwitch* rotarySwitch,
    ToggleSwitch* directionSwitch,
    PushButtonRotaryEncoder* rotaryEncoder
)
    : TrainController(motorController, display),
      _rotarySwitch(rotarySwitch),
      _directionSwitch(directionSwitch),
      _rotaryEncoder(rotaryEncoder),
      _lastUpdateTime(0),
      _acceleratonRate(0),
      _isAccelerating(false),
      _isDecelerating(false),
      _minSpeed(1000) {
}

void MasconTrainController::begin() {
    _lastUpdateTime = millis();
    
    // 初期状態を読み取り
    RotarySwitchPosition position = _rotarySwitch->getCurrentPosition();
    MasconMapping control = mapPositionToControl(position);
    bool isFwd = _directionSwitch->read();
    
    _currentState.speed = 0;
    _currentState.isForward = isFwd;
    _currentState.displayName = control.displayName;
    _currentState.switchPosition = position;
    
    _acceleratonRate = control.acceleratonRate;
    
    _display->setLines("Mascon Mode", "Ready");
    delay(1000);
}

void MasconTrainController::update() {
    unsigned long currentTime = millis();
    
    // 更新間隔チェック
    if (currentTime - _lastUpdateTime < UPDATE_INTERVAL) {
        return;
    }
    
    // スピードエンコーダーのボタン状態チェック
    if (_rotaryEncoder) {
        _rotaryEncoder->isButtonPressed();
    }
    
    // ロータリースイッチの状態を読み取り
    RotarySwitchPosition newPosition = _rotarySwitch->read();
    bool isFwd = _directionSwitch->read();
    
    // 新しいマスコン制御状態にマッピング
    MasconMapping control = mapPositionToControl(newPosition);
    _acceleratonRate = control.acceleratonRate;
    
    // 速度制御の更新（マスコン風の滑らかな制御）
    updateSpeedControl();
    
    // 状態更新
    _currentState.isForward = isFwd;
    _currentState.displayName = control.displayName;
    _currentState.switchPosition = newPosition;
    

    applyMotorControl();

    updateDisplay();
    _lastState = _currentState;
    _lastUpdateTime = currentTime;
    //delay(20);
}

void MasconTrainController::updateDisplay() {
    // 表示の更新（位置が変わった場合、または速度が変化中）
    if (_lastState.switchPosition != _currentState.switchPosition ||
        _lastState.isForward != _currentState.isForward) {
        String line1 = _currentState.displayName;
        line1 += " " + String(_currentState.isForward ? "FWD" : "REV");
        _display->setLine1(line1);
    }

    if (_isAccelerating || _isDecelerating) {
        String line2 = "SPD: " + String(_currentState.speed / 100) + "." + String(_currentState.speed % 100) + "%";
        if (_isAccelerating) {
            line2 += " ACC";
        } else if (_isDecelerating) {
            line2 += " BRK";
        }
        _display->setLine2(line2);
    }
}

void MasconTrainController::updateSpeedControl() {
    int currentSpeed = _currentState.speed;
    // 現在速度を目標速度に向けて調整（マスコン風の滑らかな制御）
    if (_acceleratonRate > 0) {
        // 加速
        currentSpeed += _acceleratonRate;
        _isAccelerating = true;
        _isDecelerating = false;
    } else if (_acceleratonRate < 0) {
        // 減速・ブレーキ
        currentSpeed += _acceleratonRate;
        _isAccelerating = false;
        _isDecelerating = true;
    } else {
        // 定速
        _isAccelerating = false;
        _isDecelerating = false;
    }
    
    // 速度範囲の制限
    if (currentSpeed < _minSpeed) currentSpeed = _minSpeed;
    if (currentSpeed > MAX_SPEED) currentSpeed = MAX_SPEED;

    _currentState.speed = currentSpeed;
}

void MasconTrainController::applyMotorControl() {
    if (_currentState.speed == 0) {
        _motorController->stop();
        return;
    }
    
    if (_currentState.isForward) {
        _motorController->forward();
    } else {
        _motorController->reverse();
    }
    
    // 周波数制御
    //int frequency = map(_currentSpeed, 0, MAX_SPEED, 120, 1800);
    int frequency = 180; // EF66, ブロアー音
    //int frequency = 330; // 千代田6000系チョッパ
    //int frequency = 10000; // 無音
    float duty = map(_currentState.speed, 0, MAX_SPEED, 0, 10000);
    
    _motorController->setPwmSettings(frequency, duty);
}

int map(int x) {
    int y = -((x-2000)^2)/4000+1000;
    return y;
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

void MasconTrainController::onRotaryEncoderEvent(RotaryDirection dir) {
    if (dir == RotaryDirection::Clockwise) {
        _minSpeed += MIN_SPEED_STEP;
    } else {
        _minSpeed -= MIN_SPEED_STEP;
    }
    if (_minSpeed < 0) _minSpeed = 0;
    if (_minSpeed > MAX_SPEED / 2) _minSpeed = MAX_SPEED / 2;
}
