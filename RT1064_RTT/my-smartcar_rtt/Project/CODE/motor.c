#include "motor.h"

#define DIR_L				D0
#define DIR_R				D2

#define PWM_L				PWM1_MODULE3_CHB_D1
#define PWM_R				PWM2_MODULE3_CHB_D3

void motor_init(void)
{
	gpio_init(DIR_L, GPO, GPIO_HIGH, GPIO_PIN_CONFIG);
	gpio_init(DIR_R, GPO, GPIO_HIGH, GPIO_PIN_CONFIG);

	pwm_init(PWM_L, 10000, 0);												// PWM 通道2 初始化频率10KHz 占空比初始为0
	pwm_init(PWM_R, 10000, 0);												// PWM 通道4 初始化频率10KHz 占空比初始为0
}

void motor_pid(int16 expect_speed)
{
    
}


void motor_control(int32 duty_l, int32 duty_r)
{
    //对占空比限幅
	duty_l = limit(duty_l, PWM_DUTY_MAX);
	duty_r = limit(duty_r, PWM_DUTY_MAX);
    
    if(duty_l >= 0)											// 左侧正转
    {
        gpio_set(DIR_L, GPIO_HIGH);							// DIR输出高电平
        pwm_duty(PWM_L, duty_l);						    // 计算占空比
    }
    else													// 左侧反转
    {
        gpio_set(DIR_L, GPIO_LOW);							// DIR输出低电平
        pwm_duty(PWM_L, -duty_l);							// 计算占空比
    }
    
    if(duty_r >= 0)											// 右侧正转
    {
        gpio_set(DIR_R, GPIO_HIGH);							// DIR输出高电平
        pwm_duty(PWM_R, duty_r);							// 计算占空比
    }
    else													// 右侧反转
    {
        gpio_set(DIR_R, GPIO_LOW);							// DIR输出低电平
        pwm_duty(PWM_R, -duty_r);							// 计算占空比
    }
}