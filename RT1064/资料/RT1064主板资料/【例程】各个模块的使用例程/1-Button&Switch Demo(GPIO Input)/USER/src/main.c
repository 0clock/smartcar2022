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
					按键&拨码开关    单片机               	         
    				SW2				查看main.c中的SW2  宏定义
					SW1             查看main.c中的SW1  宏定义
					S2              查看main.c中的KEY1 宏定义
					S3              查看main.c中的KEY2 宏定义
					S4              查看main.c中的KEY3 宏定义
					S5              查看main.c中的KEY4 宏定义
					------------------------------------
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完


// 本例程推荐使用1.8寸TFT显示屏观察效果，若您没有1.8寸TFT屏，可以通过调试时的Watch窗口观察按键状态。
// 通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看 key1_status key2_status key3_status key4_status 的数值即可。
// 使用方法，右键点击变量例如 key1_status ，Add to Live Watch(IAR) 或 add‘key1_status'to...->watch1(MDK),即可在watch窗口看到变量实时值。
// 在本例程中，当按键按下时，key1_status 变为0。


#include "SEEKFREE_FONT.h"
#include "headfile.h"

//定义按键引脚
#define KEY1    C31
#define KEY2    C27
#define KEY3    C26
#define KEY4    C4
//定义拨码开关引脚
#define SW1     D4
#define SW2     D27


//拨码开关状态变量
uint8 sw1_status;
uint8 sw2_status;

//开关状态变量
uint8 key1_status = 1;
uint8 key2_status = 1;
uint8 key3_status = 1;
uint8 key4_status = 1;

//上一次开关状态变量
uint8 key1_last_status;
uint8 key2_last_status;
uint8 key3_last_status;
uint8 key4_last_status;

//开关标志位
uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;

uint8 test1,test2,test3,test4;

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //显示模式设置为2  横屏模式 
    //显示模式在tft的h文件内的TFT_DISPLAY_DIR宏定义设置
    lcd_init();     //初始化TFT屏幕
    //如果屏幕没有变白，请检查屏幕接线
    //按键初始化
    gpio_init(KEY1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY2,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY3,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(KEY4,GPI,0,GPIO_PIN_CONFIG);
    
    //拨码开关初始化
    gpio_init(SW1,GPI,0,GPIO_PIN_CONFIG);
    gpio_init(SW2,GPI,0,GPIO_PIN_CONFIG);
    
    EnableGlobalIRQ(0);
    lcd_display_chinese(0,0,16,chinese_test[0],4,RED);
    while(1)
    {
        //获取拨码开关状态
        sw1_status = gpio_get(SW1);
        sw2_status = gpio_get(SW2);

        //在TFT上显示拨码开关状态
        lcd_showstr(0,1,"SW1 STATUS:");     lcd_showint32(12*8,1,sw1_status,1);
        lcd_showstr(0,2,"SW2 STATUS:");     lcd_showint32(12*8,2,sw2_status,1);
        
        //使用此方法优点在于，不需要使用while(1) 等待，避免处理器资源浪费
        //保存按键状态
        key1_last_status = key1_status;
        key2_last_status = key2_status;
        key3_last_status = key3_status;
        key4_last_status = key4_status;
        //读取当前按键状态
        key1_status = gpio_get(KEY1);
        key2_status = gpio_get(KEY2);
        key3_status = gpio_get(KEY3);
        key4_status = gpio_get(KEY4);
        
        //检测到按键按下之后  并放开置位标志位
        if(key1_status && !key1_last_status)    key1_flag = 1;
        if(key2_status && !key2_last_status)    key2_flag = 1;
        if(key3_status && !key3_last_status)    key3_flag = 1;
        if(key4_status && !key4_last_status)    key4_flag = 1;
        
        //标志位置位之后，可以使用标志位执行自己想要做的事件
        if(key1_flag)   
        {
            key1_flag = 0;//使用按键之后，应该清除标志位
            test1++;
        }
        
        if(key2_flag)   
        {
            key2_flag = 0;//使用按键之后，应该清除标志位
            test2++;
        }
        
        if(key3_flag)   
        {
            key3_flag = 0;//使用按键之后，应该清除标志位
            test3++;
        }
        
        if(key4_flag)   
        {
            key4_flag = 0;//使用按键之后，应该清除标志位
            test4++;
        }
        
        //在TFT上显示测试变量
        lcd_showstr(0,3,"KEY1  TEST:");     lcd_showint32(12*8,3,test1,1);
        lcd_showstr(0,4,"KEY2  TEST:");     lcd_showint32(12*8,4,test2,1);
        lcd_showstr(0,5,"KEY3  TEST:");     lcd_showint32(12*8,5,test3,1);
        lcd_showstr(0,6,"KEY4  TEST:");     lcd_showint32(12*8,6,test4,1);
        
        
        systick_delay_ms(10);//延时，按键程序应该保证调用时间不小于10ms
        
    }

    
}





