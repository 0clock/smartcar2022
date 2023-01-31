#include "smotor.h"

#define SMOTOR1_PIN   PWM4_MODULE2_CHA_C30       //定义车模寻迹舵机引脚
#define SMOTOR2_PIN   PWM1_MODULE0_CHA_D12       //定义云台舵机1引脚
#define SMOTOR3_PIN   PWM1_MODULE0_CHB_D13       //定义云台舵机2引脚
#define ELECTROMAG_PIN B25


#define SMOTOR1_CENTER  (1.5*50000/20)
#define SMOTOR2_CENTER  (1.5*50000/20)
#define SMOTOR3_CENTER  (1.5*50000/20)


void smotor_init(void)
{
    pwm_init(SMOTOR1_PIN, 50, SMOTOR1_CENTER+1600);
    gpio_init(ELECTROMAG_PIN,GPO,0,GPIO_PIN_CONFIG);
    //smotor1_control(2800);
/*    pwm_init(SMOTOR2_PIN, 50, SMOTOR2_CENTER);
    pwm_init(SMOTOR3_PIN, 50, SMOTOR3_CENTER);*/
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

void grab_picture(){
    //这里控制电磁铁吸附
    gpio_set(ELECTROMAG_PIN,1);
    //舵机测试，范围大约在-300~2800 比较合适，但是逐飞的舵机负载能力较弱，有点不稳。
    smotor1_control(-800);
    rt_thread_delay(1000);
    smotor1_control(1600);
}

void place_picture(){
    smotor1_control(0);
    //这里控制电磁铁松开
    systick_delay_ms(100);
    gpio_set(ELECTROMAG_PIN,0);
    smotor1_control(1600);
}