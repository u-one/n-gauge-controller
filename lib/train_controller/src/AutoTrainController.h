#ifndef AUTOTRAINCONTROLLER_H
#define AUTOTRAINCONTROLLER_H

#include "TrainController.h"

class AutoTrainController : public TrainController {
public:
    AutoTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display);
    
    void begin() override;
    void update() override;
};

#endif