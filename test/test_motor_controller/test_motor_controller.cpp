#include <unity.h>
#include "TestableMotorController.h"
#include "MockHardware.h"

MockHardware* mockHardware;
TestableMotorController* motorController;

const int TEST_IN1_PIN = 8;
const int TEST_IN2_PIN = 9;
const int TEST_PWM_PIN = 10;

void setUp() {
    mockHardware = new MockHardware();
    motorController = new TestableMotorController(TEST_IN1_PIN, TEST_IN2_PIN, TEST_PWM_PIN, mockHardware);
    mockHardware->reset();
}

void tearDown() {
    delete motorController;
    delete mockHardware;
}

// 初期化テスト
void test_motor_controller_setup() {
    motorController->setup();
    
    // ピンモードが正しく設定されているか確認
    TEST_ASSERT_EQUAL(1, mockHardware->getPinMode(TEST_IN1_PIN));  // OUTPUT
    TEST_ASSERT_EQUAL(1, mockHardware->getPinMode(TEST_IN2_PIN));  // OUTPUT
    TEST_ASSERT_EQUAL(1, mockHardware->getPinMode(TEST_PWM_PIN));  // OUTPUT
}

// 前進テスト
void test_motor_controller_forward() {
    motorController->setup();
    motorController->forward();
    
    // IN1=HIGH, IN2=LOWで前進
    TEST_ASSERT_EQUAL(1, mockHardware->getDigitalPinValue(TEST_IN1_PIN));  // HIGH
    TEST_ASSERT_EQUAL(0, mockHardware->getDigitalPinValue(TEST_IN2_PIN));  // LOW
}

// 後進テスト
void test_motor_controller_reverse() {
    motorController->setup();
    motorController->reverse();
    
    // IN1=LOW, IN2=HIGHで後進
    TEST_ASSERT_EQUAL(0, mockHardware->getDigitalPinValue(TEST_IN1_PIN));  // LOW
    TEST_ASSERT_EQUAL(1, mockHardware->getDigitalPinValue(TEST_IN2_PIN));  // HIGH
}

// 停止テスト
void test_motor_controller_stop() {
    motorController->setup();
    motorController->forward();  // まず動作状態にする
    motorController->stop();
    
    // 両方のピンがLOW、PWMも0
    TEST_ASSERT_EQUAL(0, mockHardware->getDigitalPinValue(TEST_IN1_PIN));  // LOW
    TEST_ASSERT_EQUAL(0, mockHardware->getDigitalPinValue(TEST_IN2_PIN));  // LOW
    TEST_ASSERT_EQUAL(0, mockHardware->getAnalogPinValue(TEST_PWM_PIN));   // 0
    TEST_ASSERT_EQUAL_FLOAT(0.0, motorController->getCurrentDutyCycle());
}

// 速度設定テスト
void test_motor_controller_speed() {
    motorController->setup();
    motorController->speed(128);
    
    TEST_ASSERT_EQUAL(128, mockHardware->getAnalogPinValue(TEST_PWM_PIN));
}

// PWM周波数設定テスト
void test_motor_controller_pwm_frequency() {
    motorController->setup();
    
    // 通常の範囲内の周波数
    motorController->setPwmFrequency(800);
    TEST_ASSERT_EQUAL(800, motorController->getCurrentFrequency());
    
    // 範囲外の周波数（下限）
    motorController->setPwmFrequency(10);
    TEST_ASSERT_EQUAL(30, motorController->getCurrentFrequency());  // 30Hz以下は30Hzに制限
    
    // 範囲外の周波数（上限）
    motorController->setPwmFrequency(3000);
    TEST_ASSERT_EQUAL(2000, motorController->getCurrentFrequency());  // 2000Hz以上は2000Hzに制限
}

// duty比設定テスト
void test_motor_controller_duty_cycle() {
    motorController->setup();
    
    // 通常の範囲内のduty比
    motorController->setDutyCycle(50.0);
    TEST_ASSERT_EQUAL_FLOAT(50.0, motorController->getCurrentDutyCycle());
    
    // 範囲外のduty比（下限）
    motorController->setDutyCycle(-10.0);
    TEST_ASSERT_EQUAL_FLOAT(0.0, motorController->getCurrentDutyCycle());
    
    // 範囲外のduty比（上限）
    motorController->setDutyCycle(150.0);
    TEST_ASSERT_EQUAL_FLOAT(100.0, motorController->getCurrentDutyCycle());
}

// PWM設定統合テスト
void test_motor_controller_pwm_settings() {
    motorController->setup();
    
    motorController->setPwmSettings(1200, 75.0);
    
    TEST_ASSERT_EQUAL(1200, motorController->getCurrentFrequency());
    TEST_ASSERT_EQUAL_FLOAT(75.0, motorController->getCurrentDutyCycle());
}

// VVVF音再現のための周波数範囲テスト
void test_motor_controller_vvvf_frequency_range() {
    motorController->setup();
    
    // VVVF音の最低周波数
    motorController->setPwmFrequency(30);
    TEST_ASSERT_EQUAL(30, motorController->getCurrentFrequency());
    
    // VVVF音の中間周波数
    motorController->setPwmFrequency(800);
    TEST_ASSERT_EQUAL(800, motorController->getCurrentFrequency());
    
    // VVVF音の最高周波数
    motorController->setPwmFrequency(2000);
    TEST_ASSERT_EQUAL(2000, motorController->getCurrentFrequency());
}

void setup() {
    UNITY_BEGIN();
    
    RUN_TEST(test_motor_controller_setup);
    RUN_TEST(test_motor_controller_forward);
    RUN_TEST(test_motor_controller_reverse);
    RUN_TEST(test_motor_controller_stop);
    RUN_TEST(test_motor_controller_speed);
    RUN_TEST(test_motor_controller_pwm_frequency);
    RUN_TEST(test_motor_controller_duty_cycle);
    RUN_TEST(test_motor_controller_pwm_settings);
    RUN_TEST(test_motor_controller_vvvf_frequency_range);
    
    UNITY_END();
}

void loop() {
    // テスト実行後は何もしない
}