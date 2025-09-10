#ifndef MASCONTRAINCONTROLLER_H
#define MASCONTRAINCONTROLLER_H

#include "TrainController.h"
#include "RotarySwitch.h"
#include "ToggleSwitch.h"

class MasconTrainController : public TrainController {
public:
    MasconTrainController(
        MotorController* motorController, 
        TwoLinesCharacterDisplay* display,
        RotarySwitch* rotarySwitch,
        ToggleSwitch* directionSwitch
    );
    
    void begin() override;
    void update() override;

private:
    RotarySwitch* _rotarySwitch;
    ToggleSwitch* _directionSwitch;
    unsigned long _lastUpdateTime;
    int _acceleratonRate;
    bool _isAccelerating;
    bool _isDecelerating;
    
    // マスコン制御パラメータ
    static const int MIN_SPEED = 1000;
    static const int MAX_SPEED = 10000;
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