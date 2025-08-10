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
private:
    int _in1Pin;
    int _in2Pin;
    int _pwmPin;
};

#endif
