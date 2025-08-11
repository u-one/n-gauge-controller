#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController {
public:
    MotorController(int in1Pin, int in2Pin, int pwmPin);
    void setup();
    void forward();
    void reverse();
    void speed(int speed);
    void stop();
    void setPwmFrequency(int frequency);
    void setDutyCycle(float dutyCycle);
    void setPwmSettings(int frequency, float dutyCycle);
    int getCurrentFrequency();
private:
    int _in1Pin;
    int _in2Pin;
    int _pwmPin;
    int _currentFrequency;
    float _currentDutyCycle;
    void _updateTimer2Settings(int frequency);
};

#endif
