/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2018,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ3184284598)
 * @version    		查看doc内version文件 版本说明
 * @Software 		IAR 8.3 or MDK 5.24
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2019-10-28
 * @note		    
 					接线定义：
					------------------------------------ 
					编码器1 							单片机
					LSB_A(步进脉冲)						QTIMER1_TIMER0_C0
					DIR  (旋转方向)或LSB_B(步进脉冲)	QTIMER1_TIMER1_C1
						
					编码器2 							单片机
					LSB_A(步进脉冲)						QTIMER2_TIMER0_C3
					DIR  (旋转方向)或LSB_B(步进脉冲)	QTIMER2_TIMER3_C25
					------------------------------------ 
 ********************************************************************************************************************/
 


//通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看 encoder1 encoder2 encoder3 encoder4 的数值即可。
//使用方法，右键点击变量例如 encoder1，Add to Live Watch(IAR) 或 add‘encoder1'to...->watch1(MDK),即可在watch窗口看到变量实时值。

#include "headfile.h"

//该例程支持带方向的编码器和需要正交解码的编码器。


//这里需要注意一下，如果是带方向输出的编码器，编码器的LSB引脚应该与A相连接，DIR引脚应该与B相连接 不可交叉

int16 encoder1;
int16 encoder2;


int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
	
	systick_delay_ms(100);	//延时100ms，等待主板其他外设上电成功
	
    //一个QTIMER可以 创建两个正交解码
    //初始化 QTIMER_1 A相使用QTIMER1_TIMER0_C0 B相使用QTIMER1_TIMER1_C1
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    //初始化 QTIMER_1 A相使用QTIMER2_TIMER0_C3 B相使用QTIMER2_TIMER3_C25
    qtimer_quad_init(QTIMER_2,QTIMER2_TIMER0_C3,QTIMER2_TIMER3_C25);
    
    EnableGlobalIRQ(0);    
    
    while(1)
    {
        //读取编码器计数值
		encoder1 = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ); //这里需要注意第二个参数务必填写A相引脚
		encoder2 = qtimer_quad_get(QTIMER_2,QTIMER2_TIMER0_C3 ); //这里需要注意第二个参数务必填写A相引脚


            
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
        qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER0_C3 );
	
    
        systick_delay_ms(100);
        //例程这里仅仅演示采集效果以及测试编码器功能是否正常，实际使用一般是将编码器的采集程序放PIT中断  周期一般设置为5ms
        
    }
}





