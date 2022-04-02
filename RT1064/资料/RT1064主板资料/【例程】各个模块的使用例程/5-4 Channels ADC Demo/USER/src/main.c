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



#include "SEEKFREE_FONT.h"
#include "headfile.h"

//定义ADC通道与引脚，其定义在zf_adc.h文件中

#define POWER_ADC1_MOD  ADC_1       //定义通道一 ADC模块号
#define POWER_ADC1_PIN  ADC1_CH3_B14//定义通道一 ADC引脚
                                                     
#define POWER_ADC2_MOD  ADC_1       //定义通道二 ADC模块号
#define POWER_ADC2_PIN  ADC1_CH4_B15//定义通道二 ADC引脚

#define POWER_ADC3_MOD  ADC_1       //定义通道三 ADC模块号
#define POWER_ADC3_PIN  ADC1_CH10_B21//定义通道三 ADC引脚

#define POWER_ADC4_MOD  ADC_1       //定义通道四 ADC模块号
#define POWER_ADC4_PIN  ADC1_CH12_B23//定义通道四 ADC引脚




uint16  ad_value1;
uint16  ad_value2;
uint16  ad_value3;
uint16  ad_value4;


int main(void)
{
    
    DisableGlobalIRQ();
    board_init();   //务必保留，本函数用于初始化MPU 时钟 调试串口
    
	systick_delay_ms(300);	//延时300ms，等待主板其他外设上电成功
	
	
    
    adc_init(POWER_ADC1_MOD,POWER_ADC1_PIN,ADC_8BIT);  //同一个ADC模块分辨率应该设置为一样的，如果设置不一样，则最后一个初始化时设置的分辨率生效
    adc_init(POWER_ADC2_MOD,POWER_ADC2_PIN,ADC_8BIT);
    adc_init(POWER_ADC3_MOD,POWER_ADC3_PIN,ADC_8BIT);
    adc_init(POWER_ADC4_MOD,POWER_ADC4_PIN,ADC_8BIT);
	
    
    EnableGlobalIRQ(0);
	
    while(1)
    {
        //采集ADC数据
        ad_value1 = adc_mean_filter(POWER_ADC1_MOD,POWER_ADC1_PIN,10);
        ad_value2 = adc_mean_filter(POWER_ADC2_MOD,POWER_ADC2_PIN,10);
        ad_value3 = adc_mean_filter(POWER_ADC3_MOD,POWER_ADC3_PIN,10);
        ad_value4 = adc_mean_filter(POWER_ADC4_MOD,POWER_ADC4_PIN,10);
		
        //通过在线调试时 Live Watch（IAR）或 Watch（MDK）功能，查看ad_value1 ad_value2 ad_value3 ad_value4的数值即可。
		//使用方法，右键点击变量例如ad_value1，Add to Live Watch(IAR) 或 add‘ad_value1'to...->watch1(MDK),即可在watch窗口看到变量实时值。
		
		//有人总是问，为啥ADC引脚悬空有数值呢？不应该是0？
		//我的回答：是的，悬空的时候是有值，这里我只给出答案，具体为什么大家自己研究ADC的结构即可明白。
		
        systick_delay_ms(10);
    }

    
}





