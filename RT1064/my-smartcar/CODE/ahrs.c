//
// Created by Jay on 2022/4/9.
//

#include "ahrs.h"
#include "fast_math.h"
#include "math.h"
#include "icm.h"

Q4_t Q4 = {1, 0, 0, 0};  //四元数
float ahrs_kp = 0.18f; //PI控制器，修正机体坐标系
float ahrs_ki = 0.01f;
float angle_offset = 0;
float cpmangle_z = 0;
vectorxyz integral;  //机体坐标误差积分
vectorxyz ahrs_angle;  //AHRS姿态解算矩阵


/*
***************************************************************
*函 数 名: AHRS_Reset()
*功能说明: AHRS相关参数重置
*形    参: 无
*返 回 值: 无
***************************************************************
*/

void AHRS_Reset(void)
{
    Q4.q0 = 1;
    Q4.q1 = 0;
    Q4.q2 = 0;
    Q4.q3 = 0;
    angle_offset = 0;
    integral.x = 0;
    integral.y = 0;
    integral.z = 0;
}

/*
***************************************************************
*函 数 名: AHRS_quat_update(vectorxyz gyro, vectorxyz acc, float interval)
*功能说明: 更新四元数
*形    参: 角速度，加速度，误差积分
*返 回 值: 结果
***************************************************************
*/
void AHRS_quat_update(vectorxyz gyro, vectorxyz acc, float interval)
{
    float q0 = Q4.q0;
    float q1 = Q4.q1;
    float q2 = Q4.q2;
    float q3 = Q4.q3;
/***********  模长  ************/
    float norm = myinvSqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
/***********  加计测出的机体坐标系   **********/
    float ax = acc.x * norm;
    float ay = acc.y * norm;
    float az = acc.z * norm;
/***********  四元数解算出的机体坐标系  ***************/
    float half_vx = q1*q3 - q0*q2;
    float half_vy = q2*q3 + q0*q1;
    float half_vz = -0.5f + q0*q0 + q3*q3;
/***********  叉积求加计机体坐标和上次四元数解算机体坐标差  ************/
    float half_ex = ay*half_vz - az*half_vy;
    float half_ey = az*half_vx - ax*half_vz;
    float half_ez = ax*half_vy - ay*half_vx;
/***********  使用PI控制器 修正机体坐标 *************/
    integral.x += half_ex * ahrs_ki * interval;
    integral.y += half_ey * ahrs_ki * interval;
    integral.z += half_ez * ahrs_ki * interval;

    float gx = (gyro.x + ahrs_kp * half_ex + integral.x) * 0.5f * interval;
    float gy = (gyro.y + ahrs_kp * half_ey + integral.y) * 0.5f * interval;
    float gz = (gyro.z + ahrs_kp * half_ez + integral.z) * 0.5f * interval;

/**********  更新四元数  ********/
    Q4.q0 += (-q1 * gx - q2 * gy - q3 * gz);
    Q4.q1 += ( q0 * gx + q2 * gz - q3 * gy);
    Q4.q2 += ( q0 * gy - q1 * gz + q3 * gx);
    Q4.q3 += ( q0 * gz + q1 * gy - q2 * gx);


    // 单位化四元数在空间旋转时不会拉伸，仅有旋转角度，下面算法类似线性代数里的正交变换
    norm = myinvSqrt(Q4.q0 * Q4.q0 + Q4.q1 * Q4.q1 + Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3);

    Q4.q0 *= norm;
    Q4.q1 *= norm;
    Q4.q2 *= norm;
    Q4.q3 *= norm;      // 用全局变量记录上一次计算的四元数值
}

/*
***************************************************************
*函 数 名: AHRS_quat_to_angle(void)
*功能说明: 更新姿态角
*形    参:无
*返 回 值: 无
***************************************************************
*/
void AHRS_quat_to_angle(void)
{
    float conv_x = 2.0f * (Q4.q0 * Q4.q2 - Q4.q1 * Q4.q3);
    float conv_y = 2.0f * (Q4.q0 * Q4.q1 + Q4.q2 * Q4.q3);
    float conv_z = Q4.q0 * Q4.q0 - Q4.q1 * Q4.q1 - Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3;
/*******  姿态解算  ********/
    ahrs_angle.x = myatan(conv_y * myinvSqrt(conv_x * conv_x + conv_z * conv_z)) * 57.2958f;
    ahrs_angle.y = asin(2 * (Q4.q0 * Q4.q2 - Q4.q3 * Q4.q1)) * 57.2958f;
    ahrs_angle.z = atan2(2 * (Q4.q0 * Q4.q3 + Q4.q1 * Q4.q2), 1 - 2 * (Q4.q2 * Q4.q2 + Q4.q3 * Q4.q3)) * 57.2958f;
/*******  角度微调  ********/
    //判断yaw角偏移自增自减
//    static uint16_t count = 0;
//    if(count >
//    angle_offset -= 0.0005585 * 0.05f;  //补偿yaw 根据自己解算速度 自行补偿  1ms解算一次 补偿
//	ahrs_angle.z += angle_offset;      //补偿yaw 根据自己解算速度 自行补偿
}
/*
***************************************************************
*函 数 名:ahrs_update(void)
*功能说明: 姿态更新
*形    参:无
*返 回 值: 无
***************************************************************
*/
void ahrs_update(void)
{
    // 读取惯性传感器数据
    icmGetValues();
    static uint32 lasttime = 0;
    float dt = (systick_getval_us() - lasttime)/1000000.0f;
    if (dt > 0.005)                                              //姿态解算周期最大6ms(可修改)
    {
        lasttime = systick_getval_us();
        return;
    }
    lasttime = systick_getval_us();

    // quat update
    //AHRS_quat_update(gyro_MovAverFilter, acc_vector, dt);   //低通滑动窗口滤波
    AHRS_quat_update(gyro_vector, acc_vector, dt);


    // 更新姿态角
    AHRS_quat_to_angle();
    cpmangle_z = complementary_filter2(ahrs_angle.z,gyro_vector.z,dt);
}

AHRS_t AHRS =
        {
                ahrs_update,
        };
/*
***************************************************************
*函 数 名: AHRS_get_yaw(void)
*功能说明: AHRS姿态解算得到yaw角
*形    参:无
*返 回 值: 无
***************************************************************
*/
void AHRS_get_yaw(void)
{
    AHRS.update();	//AHRS姿态结算
    Car.Angel=cpmangle_z;
}