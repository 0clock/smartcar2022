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

extern int RecModeTest;
//-----------------------宏-----------------------//
//-------------------结构体-----------------------//
struct Location_Goal Car={0};          //小车状态（位置，目标）存储结构体
struct Route_Dist Route_D[5];
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
int cmpFunc(const void *aa, const void *bb){//判断函数，现在是从小到大
    return (*(Route_Dist*)aa).dist>(*(Route_Dist*)bb).dist?1:-1;
}

float dis(int aNum){
    float tmpDis;
		//tmpDis= sqrt(pow(Car_Location[aNum][0],2)+ pow(Car_Location[aNum][1],2));   
    return tmpDis;
}

void Location_Route(){

    for(int location_count=0;location_count<locate_sz;++location_count){
        Route_D[location_count].dist = dis(location_count);
        Route_D[location_count].num=location_count;
    }

    qsort(Route_D,locate_sz, sizeof(struct Route_Dist),cmpFunc);//排序


//    for(int location_count=0;location_count<locate_sz;++location_count) {
//        Car_Location_Route[location_count][0] = Car_Location[Route_D[location_count].num][0];
//        Car_Location_Route[location_count][1] = Car_Location[Route_D[location_count].num][1];
//    }
}

/*
 * 识别模式
 */
void Car_RecMode(){
    do{
      Beep_Set(10,2);
			Car_Stop();
		}while(RecModeTest);
    RecModeTest=1;
}

void Car_OmniMove(){
    Car_SpeedGet();
    Car_Omni(Car.Speed_X,Car.Speed_Y,Car.Speed_Z);
    if(abs(Car.MileageX)>abs(Car.DistanceX)&&abs(Car.MileageY)>abs(Car.DistanceY)){
        Car_Stop();
        Car_RecMode();
        Beep_Set(50,1);
        Get_Location();
        Car.MileageX=0;
        Car.MileageY=0;
    }
}

void Car_Return(){

}

/*先转向再跑，暂时不用
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
*/


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

void Get_Target(void) {
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

void Get_Location(void){
    Car.Position_Pointer++;
    if(Car.Position_Pointer>=locate_sz)
        Car.Position_Pointer=locate_sz;

    Charge_Locate();
    Get_Target();
    //用两点式计算角度和距离
    Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
    Car.DistanceX=20*(Car.x1-Car.x);
    Car.DistanceY=20*(Car.y1-Car.y);
}


