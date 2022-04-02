/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2019,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Target core		NXP RT1064DVL6A
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-04-30
 * @note
					接线定义：
					------------------------------------ 
					舵机         单片机
					PWM			 查看main.c文件中的PWM4_MODULE2_CHA_C30宏定义
					------------------------------------ 
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "SEEKFREE_FONT.h"
#include "headfile.h"

#define S_MOTOR1_PIN   PWM4_MODULE2_CHA_C30       //定义舵机引脚
#define S_MOTOR2_PIN   PWM1_MODULE0_CHA_D12       //定义舵机引脚
#define S_MOTOR3_PIN   PWM1_MODULE0_CHB_D13       //定义舵机引脚


uint16 duty;
int main(void)
{
    
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
	
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //舵机居中   1.5ms/20ms * PWM_DUTY_MAX（PWM_DUTY_MAX是PWM的满占空比时候的值） PWM_DUTY_MAX在fsl_pwm.h文件中 默认为50000
    duty = 1.5*50000/20;
    
    pwm_init(S_MOTOR1_PIN,50,duty);
    pwm_init(S_MOTOR2_PIN,50,duty);
    pwm_init(S_MOTOR3_PIN,50,duty);
    //特别提示这里的舵机居中是在没有安装在车模上的时候，才能使用这个居中方式。
    //如果舵机已经安装在车模上，切勿使用本程序，因为舵机由于机械限位，导致可活动范围小很多。
	
    EnableGlobalIRQ(0);

    while(1)
    {
        //计算舵机位置舵机位置   （0.5ms - 2.5ms）ms/20ms * 50000（50000是PWM的满占空比时候的值）
        //舵机最小值为1250   最大值为6250
        //这里加一百 减一百是为了缩小转动的区域，不要转动过于极限位置，有的舵机在极限位置容易卡住
        duty += 10;
        if((6250-100) < duty) duty = (1250+100);
        
        //控制舵机达到指定位置
        pwm_duty(S_MOTOR1_PIN,duty);
        pwm_duty(S_MOTOR2_PIN,duty);
        pwm_duty(S_MOTOR3_PIN,duty);
        
        if((1250+100) >= duty)  systick_delay_ms(1000); //当从6250的位置   回转到1250的位置时   应该使用较长的延时等到舵机达到指定位置
        else                    systick_delay_ms(3);    //
    }

    
}





