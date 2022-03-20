/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-01-09
 * @note		
 					接线定义：
					------------------------------------ 
                    模块管脚            单片机管脚
                    1DIR                请查看main.c中的DIR_1宏定义
                    1PWM                请查看main.c中的PWM_1宏定义
					2DIR                请查看main.c中的DIR_2宏定义
                    2PWM                请查看main.c中的PWM_2宏定义
 ********************************************************************************************************************/



//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完

//下载代码前请根据自己使用的下载器在工程里设置下载器为自己所使用的

#include "headfile.h"

#define DIR_1 D0
#define DIR_2 D1
#define PWM_1 PWM2_MODULE3_CHA_D2
#define PWM_2 PWM2_MODULE3_CHB_D3

#define DIR_3 D14
#define DIR_4 D15
#define PWM_3 PWM1_MODULE0_CHA_D12
#define PWM_4 PWM1_MODULE0_CHB_D13

uint8 dir;
int32 duty;

int main(void)
{
	DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
	
	//此处编写用户代码(例如：外设初始化代码等)
	
	gpio_init(DIR_1, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D0 初始化DIR_1			GPIO
	gpio_init(DIR_2, GPO, 0, GPIO_PIN_CONFIG); 		//单片机端口D1 初始化DIR_2			GPIO
	pwm_init(PWM_1, 17000, 0);      				//单片机端口D2 初始化PWM_1周期10K 占空比0
	pwm_init(PWM_2, 17000, 0);     					//单片机端口D3 初始化PWM_2周期10K 占空比0
    
    gpio_init(DIR_3, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D0 初始化DIR_1          GPIO
    gpio_init(DIR_4, GPO, 0, GPIO_PIN_CONFIG);      //单片机端口D1 初始化DIR_2          GPIO
    pwm_init(PWM_3, 17000, 0);                      //单片机端口D2 初始化PWM_1周期10K 占空比0
    pwm_init(PWM_4, 17000, 0);                      //单片机端口D3 初始化PWM_2周期10K 占空比0
	
	
	//总中断最后开启
    EnableGlobalIRQ(0);
    while (1)
    {
		if(dir)
        {
            duty += 10;
            if(50000<=duty)   dir = 0;
        }
        else
        {
            duty -= 10;
            if(-50000>=duty)   dir = 1;
        } 
		
        systick_delay_ms(1);
       
        if(duty>=0) //正转
        {
			gpio_set(DIR_1,0);
            pwm_duty(PWM_1,duty);
			
			gpio_set(DIR_2,0);
            pwm_duty(PWM_2,duty);

            gpio_set(DIR_3,0);
            pwm_duty(PWM_3,duty);
            
            gpio_set(DIR_4,0);
            pwm_duty(PWM_4,duty);
        }
        else        //反转
        {
			gpio_set(DIR_1,1);
            pwm_duty(PWM_1,-duty);
			
			gpio_set(DIR_2,1);
            pwm_duty(PWM_2,-duty);

            gpio_set(DIR_3,1);
            pwm_duty(PWM_3,-duty);
            
            gpio_set(DIR_4,1);
            pwm_duty(PWM_4,-duty);
        }
    }
}





