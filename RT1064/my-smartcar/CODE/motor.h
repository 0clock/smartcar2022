#ifndef _MOTOR_H_
#define _MOTOR_H_

//#define DIR_1 D0
//#define DIR_2 D1
//#define PWM_1 PWM2_MODULE3_CHA_D2
//#define PWM_2 PWM2_MODULE3_CHB_D3

//#define DIR_3 D14
//#define DIR_4 D15
//#define PWM_3 PWM1_MODULE0_CHA_D12
//#define PWM_4 PWM1_MODULE0_CHB_D13

#define DIR_1 D14
#define DIR_2 D0
#define DIR_3 D1
#define DIR_4 D15
#define PWM_1 PWM1_MODULE0_CHA_D12
#define PWM_2 PWM2_MODULE3_CHA_D2
#define PWM_3 PWM2_MODULE3_CHB_D3
#define PWM_4 PWM1_MODULE0_CHB_D13

void motorinit(void);
void motorctrl(void);

void encoderinit(void);
void getencoder(void);

#endif