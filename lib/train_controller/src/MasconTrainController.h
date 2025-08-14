#ifndef MASCONTRAINCONTROLLER_H
#define MASCONTRAINCONTROLLER_H

#include "TrainController.h"

class MasconTrainController : public TrainController {
public:
    MasconTrainController(MotorController* motorController, TwoLinesCharacterDisplay* display);
    
    void begin() override;
    void update() override;
    TrainControlState getCurrentState() override;
    bool hasStateChanged() override;
};

#endif