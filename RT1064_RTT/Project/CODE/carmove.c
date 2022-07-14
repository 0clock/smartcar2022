//
// Created by Jay on 2022/7/11.
//
#include "carmove.h"
#include "location.h"
#include "motor.h"
#include "buzzer.h"
#include "attitude_solution.h"
#include "smotor.h"
extern const float PI;
int nextpoint=0;

/*
***************************************************************
*	函 数 名: Charge_Locate
* 功能说明: 坐标判断得到当前的坐标,在行进模式中循环进行？
*	形    参: 无
*	返 回 值: 自身坐标
***************************************************************
*/

void charge_locate(void)
{
    Car.x=Car.x1;
    Car.y=Car.y1;
//    //获取当前坐标
//    if (Car.Position_Pointer == 0){ //位于起点1
//        Car.x=0;
//        Car.y=0;
//    }else if(Car.Position_Pointer < locate_sz){
//        Car.x=Car_Location_Route[Car.Position_Pointer-1][0];
//        Car.y=Car_Location_Route[Car.Position_Pointer-1][1];
//    } else if (Car.Position_Pointer==locate_sz){
//        Car.x=Car_Location_Route[locate_sz-1][0];
//        Car.y=Car_Location_Route[locate_sz-1][1];
//    }

/*    Car.x+=Car.MileageX/20;
    Car.y+=Car.MileageY/20;*/
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

void get_target() {
    //赋予新的目标坐标点
    nextpoint=locate_route();
    Car.x1=originMap[nextpoint][0];
    Car.y1=originMap[nextpoint][1];
    if(Car.Position_Pointer>=locate_sz){
        Car.x1=0;
        Car.y1=0;
    }
//    if(Car.Position_Pointer<locate_sz){
//        Car.x1=Car_Location_Route[Car.Position_Pointer][0];
//        Car.y1=Car_Location_Route[Car.Position_Pointer][1];
//    }else if(Car.Position_Pointer==locate_sz){
//        Car.x1=0;
//        Car.y1=0;
//    }

}

/*
***************************************************************
*	函 数 名: Get_Road
* 功能说明: 计算路径：角度和里程
*	形    参: 无
*	返 回 值: 无
***************************************************************
*/

void get_location(void){
    charge_locate();
    get_target();
    //用两点式计算角度和距离
    Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
    Car.DistanceX=20*(Car.x1-Car.x);
    Car.DistanceY=20*(Car.y1-Car.y);
}

void omni_banyun(){
    //车头向前将图片搬运到指定位置

    //这里要改变Car.x,Car.y
}

void car_recmode(){
    //根据图片位置改变车身位置,本来打算通过总钻风来识别位置的，但是我太菜了，目前的想法是通过openart来识别图片用过串口发来位置再矫正位置
    rt_thread_delay(2500);
    grab_picture();//等待识别完成后电磁铁吸取图片
    //rt_thread_delay(2500);
    rt_mb_send(buzzer_mailbox,233);
    //omni_banyun();//吸取完成后搬运图片到指定位置
}


void car_omnimove(){
    bool x_flag,y_flag;
    Car.Angel=eulerAngle.yaw;
    if(abs(Car.MileageX)<abs(Car.DistanceX)){
        Car.Speed_X=(int16)(speed_tar * sin(Car.Angel_Target/180 *PI));//((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
        x_flag=false;
    }else{
        Car.Speed_X=0;
        x_flag=true;
    }
    if(abs(Car.MileageY)<abs(Car.DistanceY)){
        Car.Speed_Y=(int16)(speed_tar * cos(Car.Angel_Target/180 *PI));
        y_flag=false;
    }else{
        Car.Speed_Y=0;
        y_flag=true;
    }
    //Car.Speed_Z=-angel_pid(  Car.Angel,-Car.Angel_Target);//速度环

    if(x_flag && y_flag){
        car_stop();
        car_recmode();
        //rt_mb_send(buzzer_mailbox,1000);
        x_flag=false;
        y_flag=false;

        get_location();
        Car.MileageX=0;
        Car.MileageY=0;
        Car.Position_Pointer++;
    }
}




