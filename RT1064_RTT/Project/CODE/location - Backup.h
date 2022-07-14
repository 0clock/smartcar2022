//
// Created by ThinkPad on 2022/6/28.
//

#ifndef _LOCATION_H
#define _LOCATION_H

#include "headfile.h"

//---------------------结构体---------------------//
//记录坐标和相对位置的结构体
typedef struct location_goal {
    //已到达的目标点
    int8_t Position_Pointer;
    //当前速度
    int Speed_X;
    int Speed_Y;
    int Speed_Z;
    //当前位置
    float x;
    float y;
    //目标位置
    float x1;
    float y1;
    //当前姿态
    float Angel;
    //目标姿态
    float Angel_Target;
    //目标距离
    float DistanceX;
    float DistanceY;
    //距上次转向之后前进的距离(里程）
    float MileageX;
    float MileageY;
    //模式标志位:1->行进模式||0->识别模式
    int8_t Mode_Flag;
}location_goal;

extern location_goal Car;

void car_omnimove(void);

void location_route(void);

#endif
