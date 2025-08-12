#ifndef ROTARYSWITCH_H
#define ROTARYSWITCH_H

/**
 * ロータリースイッチの状態を表すenum
 */
enum class RotarySwitchStatus {
    Unknown,
    State1,
    State2,
    State3,
    State4,
    State5,
    State6,
    State7,
    State8,
    State9,
    State10,
    State11,
    State12
};

/**
 * ロータリースイッチの状態判定を行うクラス
 * アナログ値から12段階のスイッチ状態を判定する
 */
class RotarySwitch {
public:
    /**
     * アナログ値からロータリースイッチの状態を判定
     * @param analogValue 0-1023の範囲のアナログ値
     * @return RotarySwitchStatus 判定されたスイッチ状態
     */
    static RotarySwitchStatus getStatus(int analogValue);
    
    /**
     * スイッチ状態から速度値を取得
     * @param status ロータリースイッチの状態
     * @return 速度値（0-80の範囲）
     */
    static int getSpeedFromStatus(RotarySwitchStatus status);
    
    /**
     * スイッチ状態から前進/後進の方向を取得
     * @param status ロータリースイッチの状態
     * @return true=前進, false=後進
     */
    static bool isForwardDirection(RotarySwitchStatus status);
    
private:
    // 各スイッチ状態に対応するアナログ値の基準値
    static const int STATE_VALUES[];
    static const int TOLERANCE = 5;  // 許容誤差
};

#endif