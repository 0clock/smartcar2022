//
// Created by Jay on 2022/4/5.
//

#ifndef _AHRS_H
#define _AHRS_H

#include "headfile.h"
#include "Filter.h"

#define Plus  1
#define Minus 2
#define Nothing 2
#define Doing 1

typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
}Q4_t;	//四元数

typedef struct
{
    float x;
    float y;
    float z;
}vectorxyz;	//自然坐标系矢量结构体

typedef struct
{
    vectorxyz a;
    vectorxyz b;
    vectorxyz c;
}Matrix_t;	//坐标矩阵因子

typedef struct
{
    void (* update)(void);

}AHRS_t;

extern vectorxyz integral;  //机体坐标误差积分
extern vectorxyz ahrs_angle;  //机体坐标误差积分
extern vectorxyz gyro_vector;
extern vectorxyz acc_vector;
extern float angle_offset;
extern float cpmangle_z;

void AHRS_Reset(void);
void AHRS_quat_update(vectorxyz gyro, vectorxyz acc, float interval);
void AHRS_quat_to_angle(void);
void ahrs_update(void);
void AHRS_get_yaw(void);
uint8_t AHRS_Offset_Judge(float angle_z,float dt);


#endif

