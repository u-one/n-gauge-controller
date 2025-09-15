#ifndef PUSHBUTTONROTARYENCODER_H
#define PUSHBUTTONROTARYENCODER_H

#include <Arduino.h>

enum class RotaryDirection {
    Clockwise,
    CounterClockwise
};

typedef void (*RotaryEventHandler)(RotaryDirection direction);
typedef void (*ButtonEventHandler)();

class PushButtonRotaryEncoder {
public:
    PushButtonRotaryEncoder(int pinCLK, int pinDT, int pinSW);
    void setup();
    void setupInterrupt(void (*interuptHandler)());
    
    // イベントハンドラー登録
    void handleRotation();

    void onRotary(RotaryEventHandler handler);
    void onButton(ButtonEventHandler handler);
    
    // 基本機能
    bool isButtonPressed();
    
    // デバッグ用
    int getRawCLK();
    int getRawDT();
    int getRawSW();

private:
    int _pinCLK;
    int _pinDT;
    int _pinSW;
    
    bool _lastButtonState;
    unsigned long _lastUpdateTime;
    RotaryEventHandler _rotaryHandler;
    ButtonEventHandler _buttonHandler;

};

#endif