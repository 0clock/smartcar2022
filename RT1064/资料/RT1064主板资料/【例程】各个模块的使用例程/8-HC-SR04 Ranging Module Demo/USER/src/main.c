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
					HC-SR04				单片机
					TRIG				查看main.c文件中的TRIG_PIN宏定义
					ECHO				查看main.c文件中的ECHO_PIN宏定义
					------------------------------------
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "headfile.h"

#define TRIG_PIN B9                //定义超声波触发引脚端口
#define ECHO_PIN B10               //定义超声波触发引脚端口

long wait_time = 0;                //超时检测变量
uint32 distance_time = 0;          //测距的时间变量
uint32 distance = 0;               //转换出来的距离  单位为厘米

int main(void)
{
    
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //初始化IO端口为输出模式且上拉
    gpio_init(TRIG_PIN, GPO, 0, GPIO_PIN_CONFIG);

    //初始化IO端口为输出模式且上拉
    gpio_init(ECHO_PIN, GPI, 0, GPIO_PIN_CONFIG);
	
    //使用pit 必须先初始化 然后在调用pit其他函数使用对应的功能
    pit_init();
    
    EnableGlobalIRQ(0);

    for(;;)
	{
        gpio_set(TRIG_PIN,1);                           //触发引脚输出高电平
        systick_delay_us(10);                           //延时10us    
        gpio_set(TRIG_PIN,0);
        
        while(!gpio_get(ECHO_PIN));                     //检测到接收引脚为高电平则开始计时
        pit_start(PIT_CH0);
        wait_time = 0;
        while(gpio_get(ECHO_PIN))                       //检测到接收引脚为低电平则结束计时
        {
            wait_time++;                                //超时检测
            systick_delay(5);
            if(wait_time>200000) break;                 //超过等待时间
        }
        if(wait_time<200000) distance_time = pit_get_us(PIT_CH0); //获取定时的时间
        else                  distance_time = 200000;
        pit_close(PIT_CH0);                             //关闭定时器以清除计时
        
        
        distance = distance_time*340/2/1000;            //计算距离  单位毫米
		
		//通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看 distance 的数值即可。
		//使用方法，右键点击变量例如 distance ，Add to Live Watch(IAR) 或 add‘distance'to...->watch1(MDK),即可在watch窗口看到变量实时值。
		
        systick_delay_ms(10);
        
	}

    
}





