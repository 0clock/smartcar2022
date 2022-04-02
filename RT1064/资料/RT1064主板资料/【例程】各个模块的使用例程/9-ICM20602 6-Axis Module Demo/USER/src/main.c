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
                        ICM20602模块管脚			单片机管脚
                        SCL					    查看SEEKFREE_ICM20602.h文件中的SPI_SCK_PIN  宏定义
                        SDA                     查看SEEKFREE_ICM20602.h文件中的SPI_MOSI_PIN 宏定义
                        SA0                     查看SEEKFREE_ICM20602.h文件中的SPI_MISO_PIN 宏定义
                        CS                      查看SEEKFREE_ICM20602.h文件中的SPI_CS_PIN   宏定义
					------------------------------------ 
 ********************************************************************************************************************/


//整套推荐IO查看Projecct文件夹下的TXT文本



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完


#include "headfile.h"


int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //本例程仅仅使用硬件SPI进行数据采集，在ICM20602文件内，有写好的硬件IIC、模拟IIC通讯方式
    
    //连接引脚可以查看ICM20602文件中的宏定义
    icm20602_init_spi();
    
    EnableGlobalIRQ(0);
    while(1)
    {
        get_icm20602_accdata_spi();
        get_icm20602_gyro_spi();
        
        systick_delay_ms(10);//延时
		
		// 通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看 icm_gyro_x icm_gyro_y icm_gyro_z icm_acc_x icm_acc_y icm_acc_z 的数值即可。
		//icm_gyro_x,icm_gyro_y,icm_gyro_z;	三轴陀螺仪值
        //icm_acc_x,icm_acc_y,icm_acc_z;	三轴加速度计值
		
		// 使用方法，在调试模式下右键点击变量例如 icm_gyro_x ，Add to Live Watch(IAR) 或 add‘icm_gyro_x'to...->watch1(MDK),即可在watch窗口看到变量实时值。
		
    }    
}





