//
// Created by Jay on 2022/7/26.
//

#ifndef MY_SMARTCAR_RTT_SENDMSG_H
#define MY_SMARTCAR_RTT_SENDMSG_H

#include "headfile.h"

typedef enum
{
    CATEGORY_NULL   = 0,        //无类别
    DOG             = 7,        //狗
    HORSE           = 8,        //马
    CAT             = 9,        //猫
    CATTLE          = 10,       //牛
    PIG             = 11,       //猪
    ORANGE          = 13,       //橙子
    APPLE           = 14,       //苹果
    DURIAN          = 15,       //榴莲
    GRAPE           = 16,       //葡萄
    BANANA          = 17,       //香蕉
    TRAINS          = 19,       //火车
    STEAMSHIP       = 20,       //轮船
    PLANE           = 21,       //飞机
    CAR             = 22,       //小轿车
    COACH           = 23,       //大客车
    CARRY           = 54,       //搬运
}category_enum;


void send_msg(UARTN_enum uartn, uint16 second, uint16 msecond, uint8 img_pos_x, uint8 img_pos_y, category_enum cg);

#endif //MY_SMARTCAR_RTT_SENDMSG_H
