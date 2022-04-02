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
                        L3G4200			    单片机
                        SCL					查看SEEKFREE_IIC.h文件内的SEEKFREE_SCL宏定义
                        SDA             	查看SEEKFREE_IIC.h文件内的SEEKFREE_SDA宏定义
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
	
	
    simiic_init();//初始化模拟IIC
    //引脚链接查看SEEKFREE_IIC h文件的引脚定义
    l3g4200d_init();
    
    EnableGlobalIRQ(0);
    while(1)
    {
        //读取陀螺仪数据
        get_l3g4200d();
        
		//通过在线调试查看 gyro_x gyro_y gyro_z
		//模块转动越快，对应轴数值越大

        systick_delay_ms(5);
    }
}





