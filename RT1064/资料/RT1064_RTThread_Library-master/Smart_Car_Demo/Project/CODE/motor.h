#ifndef _motor_h
#define _motor_h

#include "headfile.h"

void motor_init(void);
void motor_pid(int16 expect_speed);
void motor_control(int32 duty_l, int32 duty_r);
    
#endif