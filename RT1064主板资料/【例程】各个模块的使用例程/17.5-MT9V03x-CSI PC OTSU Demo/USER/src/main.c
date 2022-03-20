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
						模块管脚         	单片机管脚
						SDA(51的RX)         查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_COF_UART_TX	宏定义
						SCL(51的TX)         查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_COF_UART_RX	宏定义
						场中断(VSY)         查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_VSYNC_PIN		宏定义
						行中断(HREF)	    不需要使用
						像素中断(PCLK)      查看SEEKFREE_MT9V03X_CSI.h文件中的MT9V03X_CSI_PCLK_PIN		宏定义
						数据口(D0-D7)		B31-B24 B31对应摄像头接口D0
						默认分辨率是        188*120
						默认FPS           	50帧
					------------------------------------ 
 ********************************************************************************************************************/



//打开新的工程或者工程移动了位置务必执行以下操作
//第一步 关闭上面所有打开的文件
//第二步 project  clean  等待下方进度条走完



#include "headfile.h"
#include "math.h"


uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row)
{
    #define GrayScale 256
    uint16 width = col;
    uint16 height = row;
    int pixelCount[GrayScale];
    float pixelPro[GrayScale];
    int i, j, pixelSum = width * height;
    uint8 threshold = 0;
    uint8* data = image;  //指向像素数据的指针
    for (i = 0; i < GrayScale; i++)
    {
        pixelCount[i] = 0;
        pixelPro[i] = 0;
    }

    //统计灰度级中每个像素在整幅图像中的个数  
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            pixelCount[(int)data[i * width + j]]++;  //将像素值作为计数数组的下标
        }
    }

    //计算每个像素在整幅图像中的比例  
    float maxPro = 0.0;
    for (i = 0; i < GrayScale; i++)
    {
        pixelPro[i] = (float)pixelCount[i] / pixelSum;
        if (pixelPro[i] > maxPro)
        {
            maxPro = pixelPro[i];
        }
    }

    //遍历灰度级[0,255]  
    float w0, w1, u0tmp, u1tmp, u0, u1, u, deltaTmp, deltaMax = 0;
    for (i = 0; i < GrayScale; i++)     // i作为阈值
    {
        w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;
        for (j = 0; j < GrayScale; j++)
        {
            if (j <= i)   //背景部分  
            {
                w0 += pixelPro[j];
                u0tmp += j * pixelPro[j];
            }
            else   //前景部分  
            {
                w1 += pixelPro[j];
                u1tmp += j * pixelPro[j];
            }
        }
        u0 = u0tmp / w0;
        u1 = u1tmp / w1;
        u = u0tmp + u1tmp;
        deltaTmp = w0 * pow((u0 - u), 2) + w1 * pow((u1 - u), 2);
        if (deltaTmp > deltaMax)
        {
            deltaMax = deltaTmp;
            threshold = i;
        }
    }

    return threshold;
}

uint8 image_threshold;  //图像阈值
uint32 use_time;     

int main(void)
{
    int i;
    uint8 *p;
    
    DisableGlobalIRQ();
    board_init();   	//务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    mt9v03x_csi_init();	//初始化摄像头 使用CSI接口
    
    pit_init();     	//使用pit 必须先初始化 然后在调用pit其他函数使用对应的功能
    
    //串口初始化 由于默认开启了printf功能，因此在board_init已经初始化了串口1 因此本例程不再初始化
    
    EnableGlobalIRQ(0);
    while(1)
    {
        if(mt9v03x_csi_finish_flag)
        {
			mt9v03x_csi_finish_flag = 0;
			
            pit_start(PIT_CH0);
            image_threshold = otsuThreshold(mt9v03x_csi_image[0],MT9V03X_CSI_W,MT9V03X_CSI_H);  //大津法计算阈值
            use_time = pit_get_us(PIT_CH0);                        //计算大津法程序消耗时间，单位微秒。
			
			//本例程大津法时间绝对很长，想直接用必然不可行，需自行优化。
			//有人说：你咋不直接优化好呢。我说：你想得美，要不要我直接把车做好给你啊。
            pit_close(PIT_CH0);

            
            //发送二值化图像至上位机
            p = mt9v03x_csi_image[0];
            uart_putchar(USART_1,0x00);uart_putchar(USART_1,0xff);uart_putchar(USART_1,0x01);uart_putchar(USART_1,0x01);//发送命令
            for(i=0; i<MT9V03X_CSI_W*MT9V03X_CSI_H; i++)
            {
                if(p[i]>image_threshold)    uart_putchar(USART_1,0xff);
                else                        uart_putchar(USART_1,0x00);
            }   
        }   
    }
}





