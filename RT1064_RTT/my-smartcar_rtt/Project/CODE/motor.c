#include "motor.h"

#define DIR_1 D14
#define DIR_2 D0
#define DIR_3 D1
#define DIR_4 D15
#define PWM_1 PWM1_MODULE0_CHA_D12
#define PWM_2 PWM2_MODULE3_CHA_D2
#define PWM_3 PWM2_MODULE3_CHB_D3
#define PWM_4 PWM1_MODULE0_CHB_D13

void motor_init(void)
{
    gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
    gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
    pwm_init(PWM_1, 17000, 0);      							//单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_2, 17000, 0);     								//单片机端口D3 初始化PWM_2周期10K 占空比0
    gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);     //单片机端口D0 初始化DIR_1          GPIO
    gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);     //单片机端口D1 初始化DIR_2          GPIO
    pwm_init(PWM_3, 17000, 0);                     //单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_4, 17000, 0);                     //单片机端口D3 初始化PWM_2周期10K 占空比0											// PWM 通道4 初始化频率10KHz 占空比初始为0
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