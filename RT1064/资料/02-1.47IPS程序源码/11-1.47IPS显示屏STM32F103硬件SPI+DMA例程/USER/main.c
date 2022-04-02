//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-10-31
//  最近修改   : 
//  功能描述   :演示例程(STM32F103系列)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   3.3v电源
//              SCL   PA5（SCLK）
//              SDA   PA7（MOSI）
//              RES   PA1
//              DC    PA2
//              CS    PA3
//              BLK   PA4
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018-10-31
//All rights reserved
//******************************************************************************/
#include "delay.h"
#include "sys.h"
#include "led.h"
#include "lcd_init.h"
#include "lcd.h"
#include "pic.h"
int main(void)
{
	float t=0;
	delay_init();
	LED_Init();//LED初始化
	LCD_Init();//LCD初始化
	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	LED=0;
	while(1)
	{
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_Fill(0,0,LCD_W,LCD_H,RED);
		LCD_Fill(0,0,LCD_W,LCD_H,GREEN);
		LCD_Fill(0,0,LCD_W,LCD_H,BLUE);
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
		LCD_ShowChinese(40,0,"中景园电子",RED,WHITE,32,0);
		LCD_ShowString(10,33,"LCD_W:",RED,WHITE,32,0);
		LCD_ShowIntNum(106,33,LCD_W,3,RED,WHITE,32);
		LCD_ShowString(10,66,"LCD_H:",RED,WHITE,32,0);
		LCD_ShowIntNum(106,66,LCD_H,3,RED,WHITE,32);
		LCD_ShowFloatNum1(10,99,t,4,RED,WHITE,32);
		t+=0.11;
		LCD_ShowPicture(160,95,40,40,gImage_1);
	}
}

