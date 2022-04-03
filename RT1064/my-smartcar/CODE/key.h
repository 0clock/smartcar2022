#ifndef _KEY_H_
#define _KEY_H_

#include "motor.h"

#define STATUS0_COUNTER_MAX 5
#define STATUS1_COUNTER_MAX 50

enum Keycode
{
    Key1,
    Key2,
    Key3,
    Key4
};

void Key_Init(void);
void Key_Scans(void);

void Key1_Action(void);
void Key2_Action(void);
void Key3_Action(void);
void Key4_Action(void);


#endif