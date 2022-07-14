#include "encoder.h"
#include "motor.h"
#include "timer_pit.h"
#include "elec.h"
#include "MahonyAHRS.h"
#include "attitude_solution.h"
#include "location.h"
#include "imgproc.h"
#include "flash_param.h"
#include "carmove.h"


void Find_Edge_1(void);

extern image_t img_raw;
extern image_t img_thres;
extern image_t img_line;

void timer1_pit_entry(void *parameter)
{
    static uint32 time;
    time++;

    //采集陀螺仪数据 
    //Mahony_computeAngles();
    ICM_getEulerianAngles();


    //获取编码器的值
    encoder_get();


    Car.Speed_Z=-angel_pid(  eulerAngle.yaw,-0);//omnimove模式下目标方向一直为0
    car_omni(Car.Speed_X,Car.Speed_Y,Car.Speed_Z);

    //控制电机转动
    motor_control(true);
}


void timer_pit_init(void)
{
    rt_timer_t timer;
    
    //创建一个定时器 周期运行
    timer = rt_timer_create("timer1", timer1_pit_entry, RT_NULL, 1, RT_TIMER_FLAG_PERIODIC);

    //启动定时器
    if(RT_NULL != timer)
    {
        rt_timer_start(timer);
    }
}