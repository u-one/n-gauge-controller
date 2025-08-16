#ifndef DEMOTRAINCONTROLLER_H
#define DEMOTRAINCONTROLLER_H

#include "TrainController.h"

class DemoTrainController : public TrainController {
public:
    DemoTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display);
    
    void begin() override;
    void update() override;
};

#endif