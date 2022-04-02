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
						摄像头FLEXIO接口    单片机管脚
						SDA(摄像头的RX)     查看SEEKFREE_SCC8660.h文件中的SCC8660_COF_UART_TX宏定义
						SCL(摄像头的TX)     查看SEEKFREE_SCC8660.h文件中的SCC8660_COF_UART_RX宏定义
                        场中断(VSY)         查看SEEKFREE_SCC8660.h文件中的SCC8660_VSYNC_PIN宏定义
						行中断(HREF)		查看SEEKFREE_SCC8660.h文件中的SCC8660_HREF_PIN宏定义
						像素中断(PCLK)      查看SEEKFREE_SCC8660.h文件中的SCC8660_PCLK_PIN宏定义
						数据口(D0-D7)		查看SEEKFREE_SCC8660.h文件中的SCC8660_DATA_PIN宏定义
					------------------------------------ 
						摄像头CSI接口       单片机管脚
						SDA(摄像头的RX)     查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_COF_UART_TX宏定义
						SCL(摄像头的TX)     查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_COF_UART_RX宏定义
						场中断(VSY)         查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_VSYNC_PIN宏定义
						行中断(HREF)        不与核心板连接（悬空）
						像素中断(PCLK)      查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_PCLK_PIN宏定义
						数据口(D0-D7)       B31-B24 B31对应摄像头接口D0
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




// 例程下载后默认显示 CSI 接口的图像，按下 C4 按键可在两个摄像头切换




#include "SEEKFREE_FONT.h"
#include "headfile.h"

#define KEY	C4

uint8 key_flag = 0u;


int main(void)
{
	DisableGlobalIRQ();
    board_init();//务必保留，本函数用于初始化MPU 时钟 调试串口
	
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
	gpio_init(KEY,GPI,0,GPIO_PIN_CONFIG);								// 初始化C4按键
	ips200_init();														// 初始化2.0寸IPS屏幕
	
	ips200_showstr(0,0,"Initialize CSI Camera...");						// 屏幕显示正在初始化CSI接口摄像头文字
	scc8660_csi_init();													// 初始化CSI接口总钻风摄像头
	ips200_showstr(0,1,"CSI Camera OK.");								// 屏幕显示初始化成功文字
	ips200_showstr(0,2,"Initialize FlexIO Camera...");					// 屏幕显示正在初始化FlexIO接口摄像头文字
	scc8660_init();														// 初始化FlexIO接口总钻风摄像头
	ips200_showstr(0,3,"FlexIO Camera OK.");							// 屏幕显示初始化成功文字
	
    EnableGlobalIRQ(0);
    while (1)
    {
		if(!gpio_get(KEY))												// 判断按键按下
		{
			systick_delay_ms(20);
			if(!gpio_get(KEY))
			{
				while(!gpio_get(KEY));
				key_flag = !key_flag;
			}
		}
		if(!key_flag && scc8660_csi_finish_flag)						//CSI摄像头采集完成
		{
			scc8660_csi_finish_flag = 0;								//清除采集完成标志位
			ips200_displayimage8660_zoom1(scc8660_csi_image[0],SCC8660_CSI_PIC_W,SCC8660_CSI_PIC_H, 0, 0, 320, 240);	//显示图像
		}
		if(key_flag && scc8660_finish_flag)								//FlexIO摄像头采集完成
		{
			scc8660_finish_flag = 0;									//清除采集完成标志位
			ips200_displayimage8660_zoom1(scc8660_image[0],SCC8660_PIC_W,SCC8660_PIC_H, 0, 0, 320, 240);	//显示图像
		}

    }
}




