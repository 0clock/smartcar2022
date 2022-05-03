//
// Created by Jay on 2022/4/7.
//
/***************************************************
*
* @file         location.c
* @brief        传感器数据处理文件
* @version      v1.0
* @date         2022/3/14
***************************************************/
#include "location.h"

//-----------------------宏-----------------------//
//-------------------结构体-----------------------//
struct Location_Goal Car={0};          //小车状态（位置，目标）存储结构体
struct Route_Dist Route_D[5];
//---------------------数组-----------------------//
int Car_Location[locate_sz][2]={//坐标原始数据
        1,20,
        3,4,
        15,20,
        7,8,
        1,5,
        0,0
};
int Car_Location_Route[locate_sz][2]={};//存放经过路径规划算法之后的坐标数据
int CarMode;

/*
 ***************************************************************
*	函 数 名: Location_Route
*   功能说明: 路径规划
*	形    参: 无
*	返 回 值: 无
 ***************************************************************
 */
int cmpFunc(const void *aa, const void *bb){//判断函数，现在是从小到大
    return (*(Route_Dist*)aa).dist>(*(Route_Dist*)bb).dist?1:-1;
}

double dis(int aNum){
    double tmpDis= sqrt(pow(Car_Location[aNum][0],2)+ pow(Car_Location[aNum][1],2));
    return tmpDis;
}

void Location_Route(){
    for(int i=0;i<locate_sz;++i){
        Route_D[i].dist= dis(i);
        Route_D[i].num=i;
       }
    qsort(Route_D,locate_sz, sizeof(struct Route_Dist),cmpFunc);//排序
    //printf("--顺序点位--\n");
    for(int i=0;i<locate_sz;++i){
        //printf("(%d,%d)\n",Car_Location[Route_D[i].num][0],Car_Location[Route_D[i].num][1]);//根据序号输出a(x,y), x=a[D[i].num][0] y=a[D[i].num][1]
        Car_Location_Route[i][0]=Car_Location[Route_D[i].num][0];
        Car_Location_Route[i][1]=Car_Location[Route_D[i].num][1];
    }
}

void Car_OmniMove(){
    if((int)Car.mileage<=(int)Car.Distance){
        Car_Omni(((float)speed_tar * sin(Car.Angel_Target/180 *PI)),((float)speed_tar * cos(Car.Angel_Target/180 *PI)),0);
    }else{
        Car_Stop();
        Get_Location();
        Car.mileage=0;
        Beep_flag=1;
    }
}

void Car_Move(){
    if((int)Car.Angel==(int)Car.Angel_Target-1 && (int)Car.Distance==(int)Car.mileage){
        Car_Stop();
        Get_Location();
        Car.mileage=0;
        Beep_flag=1;
    }
    if(Car.Angel!=(int)Car.Angel_Target){
        if(0<Car.Angel_Target-1-Car.Angel){
            Car_Turnround();
            CarMode=anticlockwise;
        }else{
            Car_Anticlockwise();
            CarMode=turnround;
        }
      }else if(Car.mileage!=Car.Distance){
        Car_Ahead();
        CarMode=ahead;
    }else{
        Car_Stop();
        CarMode=stop;
    }
}


void Car_Mode(){
    switch (CarMode) {
        case ahead:Car_Ahead();break;
        case forward:Car_Back();break;
        case turnround:Car_Turnround();break;
        case anticlockwise:Car_Anticlockwise();break;
        case stop:Car_Stop();break;
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

void Charge_Locate(void)
{
    //获取当前坐标
    Car.x+=Car.mileage*sin(Car.Angel);
    Car.y+=Car.mileage*sin(Car.Angel);
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

void Get_Target(void)
{
    //赋予新的目标坐标点
    Car.x=Car_Location[Car.Position_Pointer-1][0];
    Car.y=Car_Location[Car.Position_Pointer-1][1];

    Car.x1=Car_Location[Car.Position_Pointer][0];
    Car.y1=Car_Location[Car.Position_Pointer][1];
    //下一个目标点
    Car.Position_Pointer++;
}
 
/*
***************************************************************
*	函 数 名: Get_Road
* 功能说明: 计算路径：角度和里程
*	形    参: 无
*	返 回 值: 无
***************************************************************
*/

void Get_Location(void)
{
    Get_Target();
    //用两点式计算角度和距离
    Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
    Car.Distance=20*sqrt((Car.x-Car.x1)*(Car.x-Car.x1)+(Car.y-Car.y1)*(Car.y-Car.y1));
}


