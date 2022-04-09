//
// Created by Jay on 2022/4/5.
//

#ifndef _ICM_H
#define _ICM_H

//#include "headfile.h"
#include "SEEKFREE_ICM20602.h"
#include "ahrs.h"



//-----------------------宏-----------------------//
//--------------------全局变量---------------------//
//---------------------结构体---------------------//
//-------------------函数定义---------------------//

void Charge_Locate(void);            //坐标处理函数

extern int16 icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16 icm_acc_x,icm_acc_y,icm_acc_z;
extern struct vectorxyz gyro_RCFilter;
extern struct vectorxyz gyro_MovAverFilter;

typedef struct IMU_param_t//角加速度输出值
{
    float acc_x;
    float acc_y;
    float acc_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;
}IMU_param_t;

extern struct IMU_param_t icmOffset;               // 陀螺仪校准值
extern struct IMU_param_t icm_data;                 // ICM处理值



//----------数据处理------------
void icmOffsetInit(void);     //零漂消除
void icmGetValues(void);       //低通+数据单位换算


#endif
