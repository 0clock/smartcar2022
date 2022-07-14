#ifndef _motor_h
#define _motor_h

#include "headfile.h"

extern int speed_tar;

void motor_init(void);
void motor_pid(int16 expect_speed);
void motor_control(bool run);
void car_omni(int16 x, int16 y, int16 z);
void car_stop();
int angel_pid(int NowAngel,int TargetAngel);
#endif