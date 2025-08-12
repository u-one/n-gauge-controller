#include "RotarySwitch.h"

// 各スイッチ状態に対応するアナログ値の基準値
const int RotarySwitch::STATE_VALUES[] = {
    -1,     // Unknown（使用しない）
    5,      // State1: 0-10の範囲
    91,     // State2: 91±5
    185,    // State3: 185±5
    278,    // State4: 278±5
    372,    // State5: 372±5
    465,    // State6: 465±5
    558,    // State7: 558±5
    651,    // State8: 651±5
    743,    // State9: 743±5
    837,    // State10: 837±5
    931,    // State11: 931±5
    1023    // State12: 1018-1023の範囲
};

RotarySwitchStatus RotarySwitch::getStatus(int analogValue) {
    // State1の特別な範囲チェック（0-10）
    if (0 <= analogValue && analogValue <= 10) {
        return RotarySwitchStatus::State1;
    }
    
    // State12の特別な範囲チェック（1018-1023）
    if (1023 - TOLERANCE <= analogValue && analogValue <= 1023) {
        return RotarySwitchStatus::State12;
    }
    
    // State2-11の範囲チェック（基準値±許容誤差）
    for (int i = 2; i <= 11; i++) {
        int baseValue = STATE_VALUES[i];
        if (baseValue - TOLERANCE <= analogValue && analogValue <= baseValue + TOLERANCE) {
            return static_cast<RotarySwitchStatus>(i);
        }
    }
    
    return RotarySwitchStatus::Unknown;
}

int RotarySwitch::getSpeedFromStatus(RotarySwitchStatus status) {
    switch(status) {
        case RotarySwitchStatus::State2:
        case RotarySwitchStatus::State10:
            return 80;
        case RotarySwitchStatus::State3:
        case RotarySwitchStatus::State9:
            return 60;
        case RotarySwitchStatus::State4:
        case RotarySwitchStatus::State8:
            return 40;
        case RotarySwitchStatus::State5:
        case RotarySwitchStatus::State7:
            return 20;
        case RotarySwitchStatus::State6:
        default:
            return 0;  // 停止、または不明な状態
    }
}

bool RotarySwitch::isForwardDirection(RotarySwitchStatus status) {
    switch(status) {
        case RotarySwitchStatus::State7:   // 前進（低速）
        case RotarySwitchStatus::State8:   // 前進（中低速）
        case RotarySwitchStatus::State9:   // 前進（中高速）
        case RotarySwitchStatus::State10:  // 前進（高速）
            return true;
        case RotarySwitchStatus::State2:   // 後進（高速）
        case RotarySwitchStatus::State3:   // 後進（中高速）
        case RotarySwitchStatus::State4:   // 後進（中低速）
        case RotarySwitchStatus::State5:   // 後進（低速）
            return false;
        case RotarySwitchStatus::State6:   // 停止
        default:
            return true;  // デフォルトは前進（停止時も前進方向を保持）
    }
}