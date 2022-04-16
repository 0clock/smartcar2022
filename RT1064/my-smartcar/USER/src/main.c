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

int8 CarFlag=1;
int32_t My_Count = 0;
uint32_t current_time = 0;
uint32_t last_time = 0;
uint32_t icm_reset_time = 0;

int main(void)
{
	DisableGlobalIRQ();
	board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	/*******************************************/
	pit_init();//
	pit_interrupt_ms(PIT_CH0,5);  //初始化pit通道0 周期5ms 编码器中断
	pit_interrupt_ms(PIT_CH1,16);  //初始化pit通道1 周期10ms	按键、串口发送中断
	pit_interrupt_ms(PIT_CH2,10);  //初始化pit通道2 周期10ms	PID控制中断
	NVIC_SetPriority(PIT_IRQn,1);
	
	
	Motor_Init();
	Encoder_Init();
    RCEncoder_Init();
	Key_Init();
	
	simiic_init();//模拟IIC端口初始化
    icm20602_init_spi();
    //icm20602_init();//icm初始化
    icmOffsetInit();//icm零漂消除
	GUI_init();
	//mt9v03x_csi_init();	//初始化摄像头 使用CSI接口
	//如果屏幕一直显示初始化信息，请检查摄像头接线
	//如果使用主板，一直卡在while(!uart_receive_flag)，请检查是否电池连接OK?
	uart_init (USART_8, 115200,UART8_TX_D16,UART8_RX_D17); //初始化串口
	
	// VOFA+ 
    VOFA* VOFA_pt = vofa_create();       //创建VOFA对象
    vofa_init(VOFA_pt,                   //初始化当前的vofa对象
                vofa_ch_data,ch_sz,
       vofa_image,image_sz,
       custom_buf,custom_sz,
       cmd_rxbuf,cmd_sz,
       USART_8,USART_8,USART_8);

	//如果图像只采集一次，请检查场信号(VSY)是否连接OK?
	systick_delay_ms(500);
	systick_start();
	EnableGlobalIRQ(0);
    Beep_flag=1;
    Car.Angel_Target=20;
	while(1)
	{
#if 0

        Car.Angel=Angel_z;
        Car_Move();
        if(Car.Angel==Car.Angel_Target){
            Get_Location();
        }
        //屏幕显示
		
		GUI_speed();
		GUI_duty();
#endif
#if 0

#endif
        //给定周期5ms解算一次

        current_time = pit_get_us(PIT_CH1) - last_time;

        if(current_time > 5000)
        {
            last_time = pit_get_us(PIT_CH1);	//更新时间
            AHRS_get_yaw();
            yaw_Filter = Movingaverage_filter(ahrs_angle.z);    //滑动窗口滤波
            icm_reset_time++;
        }
        if(icm_reset_time > 2000)   //10s后重新初始化
        {
            icm20602_init_spi();            //icm重新初始化
            icmOffsetInit();               //消除零漂
            AHRS_Reset();
            icm_reset_time = 0;
        }
        GUI_icm20602();
    }
}




