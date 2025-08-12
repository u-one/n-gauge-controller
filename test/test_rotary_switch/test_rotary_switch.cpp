#include <unity.h>
#include "RotarySwitch.h"

// 状態判定テスト - 正確な基準値でのテスト
void test_rotary_switch_exact_values() {
    // State1の境界値テスト
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State1, RotarySwitch::getStatus(0));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State1, RotarySwitch::getStatus(5));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State1, RotarySwitch::getStatus(10));
    
    // State2-11の基準値テスト
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State2, RotarySwitch::getStatus(91));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State3, RotarySwitch::getStatus(185));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State4, RotarySwitch::getStatus(278));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State5, RotarySwitch::getStatus(372));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State6, RotarySwitch::getStatus(465));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State7, RotarySwitch::getStatus(558));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State8, RotarySwitch::getStatus(651));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State9, RotarySwitch::getStatus(743));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State10, RotarySwitch::getStatus(837));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State11, RotarySwitch::getStatus(931));
    
    // State12の境界値テスト
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State12, RotarySwitch::getStatus(1018));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State12, RotarySwitch::getStatus(1023));
}

// 許容誤差範囲でのテスト
void test_rotary_switch_tolerance_range() {
    // State2の許容誤差テスト（91±5）
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State2, RotarySwitch::getStatus(86));  // 91-5
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State2, RotarySwitch::getStatus(96));  // 91+5
    
    // State5の許容誤差テスト（372±5）
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State5, RotarySwitch::getStatus(367)); // 372-5
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State5, RotarySwitch::getStatus(377)); // 372+5
    
    // State10の許容誤差テスト（837±5）
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State10, RotarySwitch::getStatus(832)); // 837-5
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State10, RotarySwitch::getStatus(842)); // 837+5
}

// 範囲外の値でのUnknownテスト
void test_rotary_switch_unknown_values() {
    // State1の範囲外
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(15));
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(50));
    
    // State間の中間値
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(140)); // State2とState3の間
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(400)); // State5とState6の間
    
    // 範囲外の許容誤差
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(85));  // State2の範囲外
    TEST_ASSERT_EQUAL(RotarySwitchStatus::Unknown, RotarySwitch::getStatus(97));  // State2の範囲外
}

// 速度値取得テスト
void test_rotary_switch_speed_values() {
    // 高速（80）
    TEST_ASSERT_EQUAL(80, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State2));
    TEST_ASSERT_EQUAL(80, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State10));
    
    // 中高速（60）
    TEST_ASSERT_EQUAL(60, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State3));
    TEST_ASSERT_EQUAL(60, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State9));
    
    // 中低速（40）
    TEST_ASSERT_EQUAL(40, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State4));
    TEST_ASSERT_EQUAL(40, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State8));
    
    // 低速（20）
    TEST_ASSERT_EQUAL(20, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State5));
    TEST_ASSERT_EQUAL(20, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State7));
    
    // 停止（0）
    TEST_ASSERT_EQUAL(0, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::State6));
    TEST_ASSERT_EQUAL(0, RotarySwitch::getSpeedFromStatus(RotarySwitchStatus::Unknown));
}

// 方向判定テスト
void test_rotary_switch_direction() {
    // 前進方向
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State7));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State8));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State9));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State10));
    
    // 後進方向
    TEST_ASSERT_FALSE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State2));
    TEST_ASSERT_FALSE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State3));
    TEST_ASSERT_FALSE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State4));
    TEST_ASSERT_FALSE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State5));
    
    // 停止時とUnknown時（デフォルト前進）
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::State6));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(RotarySwitchStatus::Unknown));
}

// 統合テスト - アナログ値から速度と方向を一括取得
void test_rotary_switch_integration() {
    // 前進高速の場合
    int analogValue = 837;  // State10
    RotarySwitchStatus status = RotarySwitch::getStatus(analogValue);
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State10, status);
    TEST_ASSERT_EQUAL(80, RotarySwitch::getSpeedFromStatus(status));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(status));
    
    // 後進中速の場合
    analogValue = 278;  // State4
    status = RotarySwitch::getStatus(analogValue);
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State4, status);
    TEST_ASSERT_EQUAL(40, RotarySwitch::getSpeedFromStatus(status));
    TEST_ASSERT_FALSE(RotarySwitch::isForwardDirection(status));
    
    // 停止の場合
    analogValue = 465;  // State6
    status = RotarySwitch::getStatus(analogValue);
    TEST_ASSERT_EQUAL(RotarySwitchStatus::State6, status);
    TEST_ASSERT_EQUAL(0, RotarySwitch::getSpeedFromStatus(status));
    TEST_ASSERT_TRUE(RotarySwitch::isForwardDirection(status));
}

void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_rotary_switch_exact_values);
    RUN_TEST(test_rotary_switch_tolerance_range);
    RUN_TEST(test_rotary_switch_unknown_values);
    RUN_TEST(test_rotary_switch_speed_values);
    RUN_TEST(test_rotary_switch_direction);
    RUN_TEST(test_rotary_switch_integration);
    
    UNITY_END();
}

void loop() {
    // テスト実行後は何もしない
}