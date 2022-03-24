#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "headfile.h"

#define DIR_1 D14
#define DIR_2 D0
#define DIR_3 D1
#define DIR_4 D15
#define PWM_1 PWM1_MODULE0_CHA_D12
#define PWM_2 PWM2_MODULE3_CHA_D2
#define PWM_3 PWM2_MODULE3_CHB_D3
#define PWM_4 PWM1_MODULE0_CHB_D13

extern int16 encoder1,encoder2,encoder3,encoder4;
extern int32 duty1,duty2,duty3,duty4;
extern int speed_tar_1,speed_tar_2,speed_tar_3,speed_tar_4;

void motorinit(void);
void motorctrl(void);
void encoderinit(void);
void getencoder(void);

#endif