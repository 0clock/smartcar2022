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
					2.0寸IPS模块管脚            单片机管脚
					D0                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D0_PIN 	宏定义
					D1                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D1_PIN 	宏定义
					D2                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D2_PIN 	宏定义
					D3                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D3_PIN 	宏定义
					D4                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D4_PIN 	宏定义
					D5                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D5_PIN 	宏定义
					D6                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D6_PIN 	宏定义
					D7                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D7_PIN 	宏定义
																						
					BL                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_BL_PIN 	宏定义
					CS                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_CS_PIN 	宏定义
					RD                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RD_PIN 	宏定义
					WR                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_WR_PIN 	宏定义
					RS                  查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RS_PIN 	宏定义
					RST                 查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RST_PIN 	宏定义

					电源引脚
					VCC 3.3V电源
					GND 电源地
					最大分辨率240*320
					------------------------------------ 
					模块管脚            单片机管脚
					SDA(摄像头的RX)     查看SEEKFREE_SCC8660.h文件中的SCC8660_COF_UART_TX宏定义
					SCL(摄像头的TX)     查看SEEKFREE_SCC8660.h文件中的SCC8660_COF_UART_RX宏定义
					场中断(VSY)         查看SEEKFREE_SCC8660.h文件中的SCC8660_VSYNC_PIN宏定义
					行中断(HREF)        不与核心板连接（悬空）
					像素中断(PCLK)      查看SEEKFREE_SCC8660.h文件中的SCC8660_PCLK_PIN宏定义
					数据口(D0-D7)       B31-B24 B31对应摄像头接口D0
					------------------------------------ 
					默认分辨率               160*120
					默认FPS                  50帧
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



#include "headfile.h"

int main(void)
{
    DisableGlobalIRQ();
    board_init();   		//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
	scc8660_init();			//初始化凌瞳摄像头 使用FLEXIO接口
	ips200_init();			//初始化2.0寸IPS屏幕
   
    EnableGlobalIRQ(0);
    while(1)
    {
        if(scc8660_finish_flag)			//图像采集完成
        {
			scc8660_finish_flag = 0;	//清除采集完成标志位
			
			//凌瞳摄像头默认采集分辨率320*240，2.0寸IPS屏分辨率与摄像头分辨率相同，直接显示采集到的图像。
			
            ips200_displayimage8660_zoom(scc8660_image[0], SCC8660_PIC_W, SCC8660_PIC_H, 320, 240);//显示凌瞳摄像头 图像
        }
    }
}





