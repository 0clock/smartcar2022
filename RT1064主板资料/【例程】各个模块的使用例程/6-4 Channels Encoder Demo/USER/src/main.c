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
// 请注意本RT1064主板 编码器4 电机驱动2 的引脚与 2.0寸IPS屏幕共用引脚。
// 在使用本例程时，取下2.0寸IPS屏。避免出现冲突。
//
//*********************************************************************************
//*********************************************************************************
//*********************************************************************************



#include "headfile.h"

int16 encoder1;
int16 encoder2;
int16 encoder3;
int16 encoder4;

int main(void)
{
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
	
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    //一个QTIMER可以 创建两个正交解码，其定义在zf_qtimer.h文件中
    //这里需要注意一下，如果是带方向输出的编码器，编码器的LSB引脚应该与A相连接 DIR引脚应该与B相连接 不可交叉
    
    //初始化 QTIMER_1 A相使用QTIMER1_TIMER0_C0 B相使用QTIMER1_TIMER1_C1
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER0_C0,QTIMER1_TIMER1_C1);
    
    //初始化 QTIMER_1 A相使用QTIMER1_TIMER2_C2 B相使用QTIMER1_TIMER3_C24
    qtimer_quad_init(QTIMER_1,QTIMER1_TIMER2_C2,QTIMER1_TIMER3_C24);
    
    qtimer_quad_init(QTIMER_2,QTIMER2_TIMER0_C3,QTIMER2_TIMER3_C25);
    qtimer_quad_init(QTIMER_3,QTIMER3_TIMER2_B18,QTIMER3_TIMER3_B19);
    
    
    //如果直接连接编码器的A B相，那么则可以直接采集编码器数据
    
    EnableGlobalIRQ(0);    
    
    while(1)
    {
        //读取编码器计数值
		encoder1 = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER0_C0 ); //这里需要注意第二个参数务必填写A相引脚
		encoder2 = qtimer_quad_get(QTIMER_1,QTIMER1_TIMER2_C2 );
		encoder3 = qtimer_quad_get(QTIMER_2,QTIMER2_TIMER0_C3 );
        encoder4 = qtimer_quad_get(QTIMER_3,QTIMER3_TIMER2_B18);
		
		//通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看 encoder1 encoder2 encoder3 encoder4 的数值即可。
		//使用方法，右键点击变量例如 encoder1，Add to Live Watch(IAR) 或 add‘encoder1'to...->watch1(MDK),即可在watch窗口看到变量实时值。
            
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER0_C0 );
        qtimer_quad_clear(QTIMER_1,QTIMER1_TIMER2_C2 );
        qtimer_quad_clear(QTIMER_2,QTIMER2_TIMER0_C3 );
        qtimer_quad_clear(QTIMER_3,QTIMER3_TIMER2_B18);		
    
        systick_delay_ms(100);
        //例程这里仅仅演示采集效果以及测试编码器功能是否正常，实际使用一般是将编码器的采集程序放PIT中断  周期一般设置为5ms
        
    }
}





