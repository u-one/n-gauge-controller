#ifndef MASCONTRAINCONTROLLER_H
#define MASCONTRAINCONTROLLER_H

#include "TrainController.h"
#include "RotarySwitch.h"

class MasconTrainController : public TrainController {
public:
    MasconTrainController(MotorController* motorController, RotarySwitch* rotarySwitch, TwoLinesCharacterDisplay* display);
    
    void begin() override;
    void update() override;

private:
    RotarySwitch* _rotarySwitch;
    unsigned long _lastUpdateTime;
    int _currentSpeed;
    int _acceleratonRate;
    bool _isAccelerating;
    bool _isDecelerating;
    
    // マスコン制御パラメータ
    static const int ACCELERATION_RATE = 1;    // 加速度 (speed/100ms)
    static const int DECELERATION_RATE = 2;    // 減速度 (speed/100ms)
    static const int MAX_SPEED = 1000;
    static const int BRAKE_RATE = 3;           // ブレーキ減速度 (speed/100ms)
    static const int UPDATE_INTERVAL = 100;    // 更新間隔(ms)
    
    struct MasconMapping {
        RotarySwitchPosition position;
        int acceleratonRate;
        String displayName;
        bool isBraking;
    };
    
    static const MasconMapping _mappings[];
    static const int _mappingCount;
    
    void updateSpeedControl();
    void updateDisplay();
    int getTargetSpeedForPosition(RotarySwitchPosition position);
    String getPositionName(RotarySwitchPosition position);
    bool isBrakingPosition(RotarySwitchPosition position);
    void applyMotorControl();
    MasconMapping mapPositionToControl(RotarySwitchPosition position);
};

#endif