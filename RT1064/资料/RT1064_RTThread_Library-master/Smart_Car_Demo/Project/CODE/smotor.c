#include "smotor.h"

#define SMOTOR1_PIN   PWM4_MODULE2_CHA_C30       //定义车模寻迹舵机引脚
#define SMOTOR2_PIN   PWM1_MODULE0_CHA_D12       //定义云台舵机1引脚
#define SMOTOR3_PIN   PWM1_MODULE0_CHB_D13       //定义云台舵机2引脚


#define SMOTOR1_CENTER  (1.5*50000/20)
#define SMOTOR2_CENTER  (1.5*50000/20)
#define SMOTOR3_CENTER  (1.5*50000/20)


void smotor_init(void)
{
    pwm_init(SMOTOR1_PIN, 50, SMOTOR1_CENTER);
    pwm_init(SMOTOR2_PIN, 50, SMOTOR2_CENTER);
    pwm_init(SMOTOR3_PIN, 50, SMOTOR3_CENTER);
}


void smotor1_control(int16 duty)
{
    pwm_duty(SMOTOR1_PIN, (int16)SMOTOR1_CENTER + duty);
}

void smotor2_control(int16 duty)
{
    pwm_duty(SMOTOR2_PIN, (int16)SMOTOR1_CENTER + duty);
}

void smotor3_control(int16 duty)
{
    pwm_duty(SMOTOR3_PIN, (int16)SMOTOR1_CENTER + duty);
}