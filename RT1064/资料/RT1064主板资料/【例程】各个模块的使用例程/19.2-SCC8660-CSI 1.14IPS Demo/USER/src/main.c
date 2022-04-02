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
						1.14IPS模块管脚      	单片机管脚
						SCL                 	查看SEEKFREE_IPS114_SPI.h文件中的IPS114_SCL_PIN		宏定义     硬件SPI引脚不可随意切换
						SDA                 	查看SEEKFREE_IPS114_SPI.h文件中的IPS114_SDA_PIN		宏定义     硬件SPI引脚不可随意切换
						RES                 	查看SEEKFREE_IPS114_SPI.h文件中的IPS114_REST_PIN	宏定义   
						DC                  	查看SEEKFREE_IPS114_SPI.h文件中的IPS114_DC_PIN		宏定义
						CS                  	查看SEEKFREE_IPS114_SPI.h文件中的IPS114_CS_PIN		宏定义     硬件SPI引脚不可随意切换
						
						电源引脚
						BL  3.3V电源（背光控制引脚，也可以接PWM来控制亮度）
						VCC 3.3V电源
						GND 电源地
						最大分辨率135*240
					------------------------------------ 
						模块管脚            单片机管脚
						SDA(摄像头的RX)     查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_COF_UART_TX宏定义
						SCL(摄像头的TX)     查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_COF_UART_RX宏定义
						场中断(VSY)         查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_VSYNC_PIN宏定义
						行中断(HREF)        不与核心板连接（悬空）
						像素中断(PCLK)      查看SEEKFREE_SCC8660_CSI.h文件中的SCC8660_CSI_PCLK_PIN宏定义
						数据口(D0-D7)       B31-B24 B31对应摄像头接口D0
					------------------------------------ 
						默认分辨率               160*120
						默认FPS                  50帧
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "headfile.h"


int main(void)
{
    DisableGlobalIRQ();
    board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
    
    //显示模式设置为3  横屏模式
    //显示模式在SEEKFREE_IPS114_SPI.h文件内的IPS114_DISPLAY_DIR宏定义设置
    ips114_init();     	//初始化IPS屏幕
    ips114_showstr(0,0,"SEEKFREE SCC8660");
    ips114_showstr(0,1,"Initializing... ");
    
    
    scc8660_csi_init();	//初始化摄像头 使用CSI接口
    //如果屏幕一直显示初始化信息，请检查摄像头接线
    //如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
    //如果图像只采集一次，请检查场信号(VSY)是否连接OK?
    

    
    ips114_showstr(0,1,"      OK...     ");
    systick_delay_ms(500);
    
    
    
    EnableGlobalIRQ(0);
    while(1)
    {
        if(scc8660_csi_finish_flag)
        {
			scc8660_csi_finish_flag = 0;
			
            //使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
			//凌瞳摄像头默认采集分辨率160*120，1.14寸IPS屏分辨率为240*135，采用缩放全屏显示。
            ips114_displayimage8660_zoom(scc8660_csi_image[0], SCC8660_CSI_PIC_W, SCC8660_CSI_PIC_H, 240, 135);
        }
    }
}






