//
// Created by Jay on 2022/4/5.
//

#include "icm.h"

//-----------------------宏-----------------------//
//--------------------全局变量---------------------//
vectorxyz gyro_vector;
vectorxyz acc_vector;
vectorxyz gyro_RCFilter;
vectorxyz gyro_MovAverFilter;

float MovAverbuf_gyrox[11] = {0};
float MovAverbuf_gyroy[11] = {0};
float MovAverbuf_gyroz[11] = {0};

//---------------------结构体---------------------//

struct RC_Para gyrox_Para = {0,0,0.6};
struct RC_Para gyroy_Para = {0,0,0.6};
struct RC_Para gyroz_Para = {0,0,0.6};

RC_Filter_pt RC_gyrox = &gyrox_Para;
RC_Filter_pt RC_gyroy = &gyroy_Para;
RC_Filter_pt RC_gyroz = &gyroz_Para;

struct IMU_param_t icmOffset={0,0,0,0,0,0};               // 陀螺仪校准值
struct IMU_param_t icm_data={0,0,0,0,0,0};                 // ICM处理值
//-------------------函数定义---------------------//

/*
***************************************************************
*函 数 名:  较零初始化
*功能说明:  通过采集一定数据求均值计算陀螺仪零点偏移值
*形    参:  无
*返 回 值:  无
***************************************************************
*/
void icmOffsetInit(void)
{
    icmOffset.acc_x = 0;
    icmOffset.acc_y = 0;
    icmOffset.acc_z = 0;

    icmOffset.gyro_x = 0;
    icmOffset.gyro_y = 0;
    icmOffset.gyro_z = 0;
    for (uint8_t i = 0; i < 100; ++i)
    {
        get_icm20602_gyro();    // 获取陀螺仪角速度
        get_icm20602_accdata();	// 获取加速度计加速度
        icmOffset.acc_x += icm_acc_x;
        icmOffset.acc_y += icm_acc_y;
        icmOffset.acc_z += icm_acc_z;
        icmOffset.gyro_x += icm_gyro_x;
        icmOffset.gyro_y += icm_gyro_y;
        icmOffset.gyro_z += icm_gyro_z;
        systick_delay_ms(5);    // 最大 1Khz
    }

    icmOffset.acc_x  /= 100;
    icmOffset.acc_y  /= 100;
    icmOffset.acc_z  /= 100;

    icmOffset.gyro_x /= 100;
    icmOffset.gyro_y /= 100;
    icmOffset.gyro_z /= 100;
}

/*
******************************************************************************************
*函 数 名:  陀螺仪数据处理
*功能说明:  将采集的数值转化为实际物理值, 并对陀螺仪进行去零漂处理@可以以此为基础加上卡尔曼滤波
*加速度计初始化配置 -> 测量范围: ±8g        对应灵敏度: 4096 LSB/g
*陀螺仪初始化配置   -> 测量范围: ±2000 dps  对应灵敏度: 16.4 LSB/dps
*gyro = (gyro_val / 16.4) °/s = ((gyro_val / 16.4) * PI / 180) rad/s
*形    参:  无
*返 回 值:  无
*******************************************************************************************
*/
void icmGetValues(void)
{
    //获取瞬时数据
    get_icm20602_gyro();
    get_icm20602_accdata();
    //加速度计
    icm_data.acc_x = (float) icm_acc_x - icmOffset.acc_x;
    icm_data.acc_y = (float) icm_acc_y - icmOffset.acc_y;
    icm_data.acc_z = (float) icm_acc_z - icmOffset.acc_z;
    //! 陀螺仪角速度必须转换为弧度制角速度: deg/s -> rad/s
    icm_data.gyro_x = ((float) icm_gyro_x - icmOffset.gyro_x) * PI / 180 / 16.4f;
    icm_data.gyro_y = ((float) icm_gyro_y - icmOffset.gyro_y) * PI / 180 / 16.4f;
    icm_data.gyro_z = ((float) icm_gyro_z - icmOffset.gyro_z) * PI / 180 / 16.4f;
    //将icm的值赋值到自然坐标系向量，以进行AHRS姿态解算
    acc_vector.x = icm_data.acc_x;
    acc_vector.y = icm_data.acc_y;
    acc_vector.z = icm_data.acc_z;

    gyro_vector.x = icm_data.gyro_x;
    gyro_vector.y = icm_data.gyro_y;
    gyro_vector.z = icm_data.gyro_z;
    //低通滤波
    gyro_RCFilter.x = RCFilter(icm_data.gyro_x,RC_gyrox);
    gyro_RCFilter.y = RCFilter(icm_data.gyro_y,RC_gyroy);
    gyro_RCFilter.z = RCFilter(icm_data.gyro_z,RC_gyroz);

    gyro_MovAverFilter.x = Movingaverage_filter(gyro_RCFilter.x,MovAverbuf_gyrox);
    gyro_MovAverFilter.y = Movingaverage_filter(gyro_RCFilter.y,MovAverbuf_gyroy);
    gyro_MovAverFilter.z = Movingaverage_filter(gyro_RCFilter.z,MovAverbuf_gyroz);
}