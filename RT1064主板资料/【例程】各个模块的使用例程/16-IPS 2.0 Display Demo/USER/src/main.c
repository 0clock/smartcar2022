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
                        D0                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D0_PIN 宏定义
                        D1                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D1_PIN 宏定义
                        D2                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D2_PIN 宏定义
                        D3                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D3_PIN 宏定义
                        D4                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D4_PIN 宏定义
                        D5                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D5_PIN 宏定义
                        D6                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D6_PIN 宏定义
                        D7                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D7_PIN 宏定义
                                                                                    
                        BL                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_BL_PIN 宏定义
                        CS                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_CS_PIN 宏定义
                        RD                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RD_PIN 宏定义
                        WR                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_WR_PIN 宏定义
                        RS                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RS_PIN 宏定义
                        RST                 查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RST_PIN 宏定义
        
                        电源引脚
                        VCC 3.3V电源
                        GND 电源地
                        最大分辨率240*320
                    ------------------------------------ 
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完





							// ！！！ 必看 ！！！！
							// ！！！ 必看 ！！！！
							// ！！！ 必看 ！！！！
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************
//
// 请注意本RT1064主板例程的2.0寸屏幕引脚与开源库中的引脚定义并不相同，
// 本主板例程为了方便使用双摄，将数据端口分配为两组不同IO内的连续4个引脚。
// 若自己绘制主板，且IO数量充足时，请尽量使用同一组IO内连续8个引脚，例如 C8-C15，可以提高显示性能。
//
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************






#include "SEEKFREE_FONT.h"
#include "headfile.h"

#include <math.h>


int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
    systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //显示模式设置为3  横屏旋转180°模式 
    //显示模式在IPS的h文件内的IPS_DISPLAY_DIR宏定义设置
    ips200_init();              //初始化IPS屏幕
			
    EnableGlobalIRQ(0);
	
    while(1)
    {
        ips200_clear(WHITE);									//清屏
        ips200_showstr(0,0,"seekfree.taobao.com"); 				//显示字符串
        ips200_showuint16(0,1,666);                				//显示一个16位无符号整数
        ips200_showint32(0,2,666,3);               				//显示一个32位有符号数  并去除无效0
        ips200_showint32(0,3,-666,3);              				//显示一个32位有符号数  并去除无效0
        ips200_showfloat(0,4,56.35,3,1);           				//显示一个浮点数        并去除整数部分无效0
        //以上函数x坐标设置是以像素为单位   y坐标是以字符为单位

        //汉字显示的X Y坐标与其他函数不一致，汉字显示函数坐标x y都是以像素为单位
        //这样可以使得位置调节更加随意
        ips200_display_chinese(0,5*16,16,chinese_test[0],4,RED);
		
		systick_delay_ms(2000);									//延时1秒
		ips200_clear(RED);
		systick_delay_ms(1000);									//延时1秒
		ips200_clear(GREEN);
		systick_delay_ms(1000);									//延时1秒
		ips200_clear(BLUE);
		systick_delay_ms(1000);									//延时1秒
    }
}





