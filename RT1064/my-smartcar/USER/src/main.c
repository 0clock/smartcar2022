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
						总钻风模块管脚      	单片机管脚
						SDA(51的RX)         	查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_COF_UART_TX	宏定义
						SCL(51的TX)         	查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_COF_UART_RX	宏定义
						场中断(VSY)         	查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_VSYNC_PIN		宏定义
						行中断(HREF)	    	不需要使用
						像素中断(PCLK)      	查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_PCLK_PIN		宏定义
						数据口(D0-D7)			B31-B24 B31对应摄像头接口D0
					------------------------------------ 
						默认分辨率是            188*120
						默认FPS                 50帧
					------------------------------------ 
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "headfile.h"

uint8 buffer[64] ;

int main(void)
{
	DisableGlobalIRQ();
	board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	/////////////////////////////////
	GUI_init();
	motorinit();
	encoderinit();
	
	simiic_init();//模拟IIC端口初始化
	icm20602_init();
	seekfree_wireless_init();//无线转串口模块初始化
	
	mt9v03x_csi_init();	//初始化摄像头 使用CSI接口
	//如果屏幕一直显示初始化信息，请检查摄像头接线
	//如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
	
	
//	// 蓝牙上位机测试－2
//  VOFA* VOFA_pt = vofa_create();       //创建VOFA对象
//  vofa_init(VOFA_pt,                   //初始化当前的vofa对象
//				vofa_ch_data,ch_sz,
//       vofa_image,image_sz,
//       custom_buf,custom_sz,
//       cmd_rxbuf,cmd_sz,
//       USART_8,USART_8,USART_8);
//			 
			 
	//如果图像只采集一次，请检查场信号(VSY)是否连接OK?
	systick_delay_ms(500);
	
	EnableGlobalIRQ(0);
	while(1)
	{
		get_icm20602_accdata();
		get_icm20602_gyro();
		getencoder();
		motorctrl();
		GUI_icm20602();
		GUI_speed();
		GUI_duty();
		
		
// 蓝牙上位机测试－1
		int len = snprintf((char *) buffer, sizeof(buffer), "encoder1=%d,%d,%d,%d\n", - encoder1,encoder2,encoder3,-encoder4);
		seekfree_wireless_send_buff(buffer,len);
//		len = snprintf((char *) buffer, sizeof(buffer), "encoder2=%d\r\n", encoder2);
//		seekfree_wireless_send_buff(buffer,len);
		
		
//		VOFA_pt->sendzip(VOFA_pt,VOFA_PROTOCOL_JUSTFLOAT,VOFA_CH_FRAME);
//VOFA_pt->sendzip(VOFA_pt,VOFA_PROTOCOL_JUSTFLOAT,VOFA_CH_FRAME);

		if(mt9v03x_csi_finish_flag)
		{
			mt9v03x_csi_finish_flag = 0;
			
			//使用缩放显示函数，根据原始图像大小 以及设置需要显示的大小自动进行缩放或者放大显示
			//总钻风采集到的图像分辨率为 188*120 ，1.14寸IPS屏显示分辨率为 240*135 ，图像拉伸全屏显示。
			//ips114_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 320, 172);	//显示摄像头图像
				
		}
	} 
}




