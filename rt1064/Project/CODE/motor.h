#ifndef _motor_h
#define _motor_h

#include "headfile.h"

extern double speed_tar;

void motor_init(void);
void motor_pid(int16 expect_speed);
void motor_control(bool run);
void car_omni(float x, float y, float z);
void car_stop();
int angel_pid(int NowAngel,int TargetAngel);
int picture_xerror_pid(int16 now_x,int16 target_x);
int picture_yerror_pid(int16 now_y,int16 target_y);
#endif