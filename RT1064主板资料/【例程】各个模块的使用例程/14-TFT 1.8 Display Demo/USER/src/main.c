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
						模块管脚            单片机管脚
						SCL                 查看TFT_SCL宏定义的引脚     硬件SPI引脚不可随意切换
						SDA                 查看TFT_SDA宏定义的引脚     硬件SPI引脚不可随意切换
						RES                 查看REST_PIN宏定义的引脚    
						DC                  查看DC_PIN宏定义的引脚  
						CS                  查看TFT_CS宏定义的引脚      硬件SPI引脚不可随意切换
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
						最大分辨率128*160
					------------------------------------ 
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完


#include "SEEKFREE_FONT.h"
#include "headfile.h"




int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //显示模式设置为2  横屏模式 
    //显示模式在tft的h文件内的TFT_DISPLAY_DIR宏定义设置
    lcd_init();     //初始化TFT屏幕
    
    EnableGlobalIRQ(0);
	
    while(1)
    {
        lcd_showstr(0,0,"seekfree.taobao.com"); //显示字符串
        lcd_showuint16(0,1,666);                //显示一个16位无符号整数
        lcd_showint32(0,2,666,3);               //显示一个32位有符号数  并去除无效0
        lcd_showint32(0,3,-666,3);              //显示一个32位有符号数  并去除无效0
        lcd_showfloat(0,4,56.35,3,1);           //显示一个浮点数        并去除整数部分无效0
        //以上函数x坐标设置是以像素为单位   y坐标是以字符为单位

        //汉字显示的X Y坐标与其他函数不一致，汉字显示函数坐标x y都是以像素为单位
        //这样可以使得位置调节更加随意
        lcd_display_chinese(0,5*16,16,chinese_test[0],4,RED);
    }
}





