//
// Created by ThinkPad on 2022/6/28.
//
#include <math.h>
#include "location.h"
#include "motor.h"
#include "attitude_solution.h"

#include <stdlib.h>
#include "buzzer.h"

#define locate_sz 7             //坐标数组数量,可能还需要更改，目标点数不确定！(5-20都有可能)


location_goal Car={0}; //小车状态（位置，目标）存储结构体

//-------------------结构体-----------------------//

struct Route_Dist{//距离结构体
    int num;//序号
    double dist;//距离
} Route_D[5];

//---------------------数组-----------------------//
float Car_Location[locate_sz][2]={//坐标原始数据
        10,15,
        1,1,
        9,8,
        2,3,
        13,20,
        3,6,
        10,10
};

float Car_Location_Route[locate_sz][2]={0,0};//存放经过路径规划算法之后的坐标数据

/*
 ***************************************************************
*	函 数 名: Location_Route
*   功能说明: 路径规划
*	形    参: 无
*	返 回 值: 无
 ***************************************************************
 */

double dis(int aNum){
    double tmpDis;
    tmpDis= sqrt(pow(Car_Location[aNum][0],2)+ pow(Car_Location[aNum][1],2));
    return tmpDis;
}
int cmpFunc(const void *aa, const void *bb){//判断函数，现在是从小到大
    return (*(struct Route_Dist*)aa).dist>(*(struct Route_Dist*)bb).dist;
}
void location_route(){

    for(int location_count=0;location_count<locate_sz;location_count++){
        Route_D[location_count].dist = dis(location_count);
        Route_D[location_count].num=location_count;
    }

    qsort(Route_D,locate_sz, sizeof(Route_D),cmpFunc);//排序 z


    for(int location_count=0;location_count<locate_sz;++location_count) {
        Car_Location_Route[location_count][0] = Car_Location[Route_D[location_count].num][0];
        Car_Location_Route[location_count][1] = Car_Location[Route_D[location_count].num][1];
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

void charge_locate(void)
{
    //获取当前坐标
    if (Car.Position_Pointer == 0){ //位于起点
        Car.x=0;
        Car.y=0;
    }else if(Car.Position_Pointer < locate_sz){
        Car.x=Car_Location_Route[Car.Position_Pointer-1][0];
        Car.y=Car_Location_Route[Car.Position_Pointer-1][1];
    } else if (Car.Position_Pointer==locate_sz){
        Car.x=Car_Location_Route[locate_sz-1][0];
        Car.y=Car_Location_Route[locate_sz-1][1];
    }

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

void get_target(void) {
    //赋予新的目标坐标点

    if(Car.Position_Pointer<locate_sz){
        Car.x1=Car_Location_Route[Car.Position_Pointer][0];
        Car.y1=Car_Location_Route[Car.Position_Pointer][1];
    }else if(Car.Position_Pointer==locate_sz){
        Car.x1=0;
        Car.y1=0;
    }

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
    Car.Position_Pointer++;
    if(Car.Position_Pointer>=locate_sz)
        Car.Position_Pointer=locate_sz;

    charge_locate();
    get_target();
    //用两点式计算角度和距离
    Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
    Car.DistanceX=20*(Car.x1-Car.x);
    Car.DistanceY=20*(Car.y1-Car.y);
}

void car_recmode(){
    //根据图片位置改变车身位置
}


void car_omnimove(){
    Car.Angel=(int)eulerAngle.yaw;
    //Car.Speed_X=(int16)(speed_tar * sin(Car.Angel_Target/180 *PI));//((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
    //Car.Speed_Y=(int16)(speed_tar * cos(Car.Angel_Target/180 *PI));
    Car.Speed_Z=-angel_pid(  Car.Angel,Car.Angel_Target);
    car_omni(0,0,Car.Speed_Z);
    if(abs(Car.MileageX)>abs(Car.DistanceX)&&abs(Car.MileageY)>abs(Car.DistanceY)){
        car_stop();
        rt_mb_send(buzzer_mailbox,1000);
        car_recmode();
        get_location();
        Car.MileageX=0;
        Car.MileageY=0;
    }
}


/*
#include "road.h"
#include "math.h"

#define PI 3.14159265358

rt_uint8_t ture_points_num = 0;

int points_map[22][2] = {0};//拍摄到的坐标点位置
int true_map[22][2] = {0};//转换后的坐标点位置
int points_label[22] = {0};//路径序列
double turn_angle[22] = {0};//转向角度
double map_gap[22] = {0};//旅程

void find_road(int in_map[][2], int *out_label) {
    double points_gap[21] = {0}; //用于存放点到点之间的距离
    int point_now = 0;           //当前指向点
    int pts_label[21] = {0};     //路径序列
    int road_best = 0;           //局部最优点号

    for (int i = 0; i <= ture_points_num; ++i) {
        for (int j = 0; j <= ture_points_num; ++j) {
            points_gap[j] = sqrt(pow((in_map[point_now][0] - in_map[j][0]), now][1] - in_map[j][1]), 2)); //计算距离
        }
        for (int j = 0; j <= ture_points_num; ++j) {
            unsigned char tag = 0;
            if (road_best == point_now && j != point_now) {
                for (int k = 0; k < i; ++k)
                    if (pts_label[k] == j) tag = 1;
                if (!tag) road_best = j;
            }
            else if (points_gap[j] < points_gap[road_best] && j != point_now) {
                for (int k = 0; k < i; ++k)
                    if (pts_label[k] == j) tag = 1;
                if (!tag) road_best = j;
            }
        }
        point_now = road_best;
        pts_label[i + 1] = road_best;
    }

    rt_memcpy(out_label, pts_label, (ture_points_num+1)*sizeof(int *));//把路径传到序列指针
}

//转换坐标
void map_change(int input[][2], int output[][2], int pts_nums, int *pts_label, int yr, int xb) {
    for (int i = 0; i <= pts_nums; ++i) {
        output[i][0] = input[pts_label[i]][0];
        output[i][1] = input[pts_label[i]][1];
    }//重新排序

    for (int i = 0; i <= pts_nums; ++i) {
        output[i][0] = abs(output[i][0] - yr);
        output[i][1] = abs(output[i][1] - xb);
    }//重新映射坐标系

    return ;
}

//转换坐标值
void map_calculate(int input[][2], double *angle, double *gap, const int pts_nums, double x_length, double y_length) {

//    x_length = (double)(Yr - Yl) / 100.0f;
//    y_length = (double)(Xb - Xt) / 100.0f;

    for (int i = 0; i <= pts_nums; ++i) {
        angle[i] = atan2(input[i+1][1] - input[i][1],
                         input[i+1][0] - input[i][0]) * 180.0f / PI;
        if(angle[i] < 0)
            angle[i] = 360.0f + angle[i];

//        for (int j = 1; j <= pts_nums; ++j) {
//          input[j][0] = input[j][0] - (x_length * 10.0f);
//          input[j][1] = input[j][1] + (y_length * 8.0f);
//        }

        gap[i] = sqrt(pow((((double) input[i][0] / x_length * 7.0f) -
                       ((double) input[i + 1][0] / x_length * 7.0f)), 2)
                       + pow((((double) input[i][1] / y_length * 5.0f) -
                       ((double) input[i + 1][1] / y_length * 5.0f)), 2));
    }
}

*/