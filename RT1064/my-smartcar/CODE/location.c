//
// Created by Jay on 2022/4/7.
//
/***************************************************
*
* @file         location.c
* @brief        传感器数据处理文件
* @version      v1.0
* @date         2022/3/14
***************************************************/
#include "location.h"
#include <string.h>
//-----------------------宏-----------------------//
//-------------------结构体-----------------------//
struct Location_Goal Car={0};          //小车状态（位置，目标）存储结构体
//---------------------数组-----------------------//
float location_X[locate_sz]={0,5,10,10};
float location_Y[locate_sz]={0,5,5,10};
int CarMode;

void Car_Move(){
    if(Car.Angel==Car.Angel_Target||Car.Distance==Car.mileage){
        Car_Stop();
    }
    if(Car.Angel!=Car.Angel_Target){
        if(0<Car.Angel_Target-Car.Angel){
            Car_Anticlockwise();
        }else{
            Car_Turnround();
        }
      }else if(Car.mileage!=Car.Distance){
        Car_Ahead();
    }else{
        Car_Stop();
    }
}


void Car_Mode(){
    switch (CarMode) {
        case ahead:Car_Ahead();break;
        case forward:Car_Back();break;
        case turnround:Car_Turnround();break;
        case anticlockwise:Car_Anticlockwise();break;
        case stop:Car_Stop();break;
    }
}

/*
***************************************************************
*	函 数 名: Charge_Locate
* 功能说明: 坐标判断得到当前的坐标,在行进模式中循环进行？
*	形    参: 无
*	返 回 值: 自身坐标
***************************************************************
*/

void Charge_Locate(void)
{
    //获取当前坐标
    Car.x+=Car.mileage*sin(Car.Angel);
    Car.y+=Car.mileage*sin(Car.Angel);
    //修正函数可以放在这后面
}

/*
***************************************************************
*	函 数 名: Get_Target
* 功能说明: 赋予结构体下一个数组元素的坐标,每次行进模式之前使用
*	形    参: 暂无
*	返 回 值: 无
***************************************************************
*/

void Get_Target(void)
{
    //赋予新的目标坐标点
    Car.x1=location_X[Car.Position_Pointer];
    Car.y1=location_Y[Car.Position_Pointer];
    //下一个目标点
    Car.Position_Pointer++;
}
 
/*
***************************************************************
*	函 数 名: Get_Road
* 功能说明: 计算路径：角度和里程
*	形    参: 无
*	返 回 值: 无
***************************************************************
*/

void Get_Location(void)
{
    Get_Target();
    //用两点式计算角度和距离
    Car.Angel_Target=tan((Car.x1-Car.x)/(Car.y1-Car.y));
    Car.Distance=20*sqrt((Car.x-Car.x1)*(Car.x-Car.x1)+(Car.y-Car.y1)*(Car.y-Car.y1));
}


