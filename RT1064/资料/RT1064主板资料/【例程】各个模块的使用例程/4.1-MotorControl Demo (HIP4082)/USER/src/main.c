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
					模块					单片机
					MOTOR1_A			    查看zf_pwn.h文件中的PWM2_MODULE3_CHA_D2     宏定义
					MOTOR1_B			    查看zf_pwn.h文件中的PWM1_MODULE3_CHA_D0     宏定义      
					MOTOR2_A			    查看zf_pwn.h文件中的PWM1_MODULE3_CHB_D1     宏定义
                    MOTOR2_B                查看zf_pwn.h文件中的PWM2_MODULE3_CHB_D3     宏定义              
                    MOTOR3_A                查看zf_pwn.h文件中的PWM1_MODULE0_CHA_D12    宏定义   
                    MOTOR3_B                查看zf_pwn.h文件中的PWM1_MODULE1_CHA_D14    宏定义  
                    MOTOR4_A                查看zf_pwn.h文件中的PWM1_MODULE0_CHB_D13    宏定义
                    MOTOR4_B                查看zf_pwn.h文件中的PWM1_MODULE1_CHB_D15    宏定义                          
					------------------------------------      
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "headfile.h"

#define MOTOR1_A   PWM2_MODULE3_CHA_D2   //定义1电机正转PWM引脚
#define MOTOR1_B   PWM1_MODULE3_CHA_D0   //定义1电机反转PWM引脚
                        
#define MOTOR2_A   PWM1_MODULE3_CHB_D1   //定义2电机正转PWM引脚
#define MOTOR2_B   PWM2_MODULE3_CHB_D3   //定义2电机反转PWM引脚
    
#define MOTOR3_A   PWM1_MODULE0_CHA_D12  //定义3电机正转PWM引脚
#define MOTOR3_B   PWM1_MODULE1_CHA_D14  //定义3电机反转PWM引脚
    
#define MOTOR4_A   PWM1_MODULE0_CHB_D13  //定义4电机正转PWM引脚
#define MOTOR4_B   PWM1_MODULE1_CHB_D15  //定义4电机反转PWM引脚

int32 speed1_power;
int32 speed2_power;
int32 speed3_power;
int32 speed4_power;
int main(void)
{
    
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //初始化电机PWM引脚
    
    //桌大大的推文中，建议电磁组电机频率选用13K-17K
    //最大占空比值PWM_DUTY_MAX 可以在fsl_pwm.h文件中修改 默认为50000
    //对于一个PWM模块 包含的所有通道只能输出频率一样 占空比不一样的 PWM RT1021只有两个PWM模块 每个模块有8个通道
	pwm_init(MOTOR1_A,17000,0);
    pwm_init(MOTOR1_B,17000,0);
    pwm_init(MOTOR2_A,17000,0);
    pwm_init(MOTOR2_B,17000,0);
    pwm_init(MOTOR3_A,17000,0);
    pwm_init(MOTOR3_B,17000,0);
    pwm_init(MOTOR4_A,17000,0);
    pwm_init(MOTOR4_B,17000,0);
    
    EnableGlobalIRQ(0);

    
    //这里一共8个通道，分别控制4个电机的正反转
    //这里需要注意，单片机是无法直接驱动电机的，单片机只能给出控制信号，将控制信号给驱动模块，驱动模块输出电压使得电机转动
    //可查看我们店铺MOS管驱动模块
    

    //设置默认速度  也可以通过在线调试直接修改此值  变化电机速度
    speed1_power = 5000;
    speed2_power = 5000;
    speed3_power = 5000;
    speed4_power = 5000;
    
    while(1)
    {
        //如何控制电机的正反转
        //每个电机都有两个驱动信号，其实就是控制这两个驱动信号的占空比（即高电平时间）
        //比如本历程中使用定时器1的0和1通道控制左电机，假设0占空比为50%，1通道为%0,那么电机正转
        //假设0占空比为0%，1通道为%50,那么电机反转，如果都为0，则电机停止
        
        if(0<=speed1_power) //电机1   正转 设置占空比为 百分之 (1000/TIMER1_PWM_DUTY_MAX*100)
        {
            pwm_duty(MOTOR1_A, speed1_power);
            pwm_duty(MOTOR1_B, 0);
        }
        else                //电机1   反转
        {
            pwm_duty(MOTOR1_A, 0);
            pwm_duty(MOTOR1_B, -speed1_power);
        }
        
        if(0<=speed2_power) //电机2   正转
        {
            pwm_duty(MOTOR2_A, speed2_power);
            pwm_duty(MOTOR2_B, 0);
        }
        else                //电机2   反转
        {
            pwm_duty(MOTOR2_A, 0);
            pwm_duty(MOTOR2_B, -speed2_power);
        }
        
        if(0<=speed3_power) //电机3   正转
        {
            pwm_duty(MOTOR3_A, speed3_power);
            pwm_duty(MOTOR3_B, 0);
        }
        else                //电机3   反转
        {
            pwm_duty(MOTOR3_A, 0);
            pwm_duty(MOTOR3_B, -speed3_power);
        }
        
        if(0<=speed4_power) //电机3   正转
        {
            pwm_duty(MOTOR4_A, speed4_power);
            pwm_duty(MOTOR4_B, 0);
        }
        else                //电机3   反转
        {
            pwm_duty(MOTOR4_A, 0);
            pwm_duty(MOTOR4_B, -speed4_power);
        }
        
    }

    
}





