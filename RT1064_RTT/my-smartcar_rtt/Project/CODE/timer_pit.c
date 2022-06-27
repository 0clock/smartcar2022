#include "encoder.h"
#include "motor.h"
#include "timer_pit.h"
#include "elec.h"

void timer1_pit_entry(void *parameter)
{
    static uint32 time;
    time++;
    
    //采集陀螺仪数据
    get_icm20602_gyro_spi();
    
    
    if(0 == (time%100))
    {
        //采集编码器数据
        encoder_get();
    }

    if(0 == (time%5))
    {
        //采集加速度数据
        get_icm20602_accdata_spi();
        
        //采集电磁信号
        elec_get();
        
        //根据电磁信号计算车身位置
        elec_calculate();
    }
    
    //控制电机转动
    motor_control(10000, 10000);
    
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