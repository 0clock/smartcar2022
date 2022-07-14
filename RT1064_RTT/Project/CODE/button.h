#ifndef _button_h
#define _button_h

#include "headfile.h"

enum Config_Action{
    MileageX,
    MileageY,
    AngelP,
    AngelI,
    AngelD,
    MotorP,
    MotorI,
    MotorD
};

extern enum Config_Action config_action;

void button_init(void);

#endif