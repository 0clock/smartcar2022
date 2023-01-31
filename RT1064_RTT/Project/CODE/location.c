//
// Created by ThinkPad on 2022/6/28.
//
#include <math.h>
#include "location.h"
#include "motor.h"
#include "attitude_solution.h"
#include <stdlib.h>
#include <string.h>
#include "buzzer.h"
#include "imgproc.h"
#include "SEEKFREE_MT9V03X_CSI.h"
#include "flash_param.h"

//**寻找边界**//

#define COLUMN_SCOPE 70 //列扫描范围 向左 & 向右 扫描
#define ROW_SCOPE 50    //行扫描范围 向上 & 向下 扫描

#define COLUMN_OFFSET 50 //列偏执 向左 & 向右 扫描时从中央分别 向上 & 向下 判定多少列
#define ROW_OFFSET 50    //行偏执 向上 & 向下 扫描时从中央分别 向左 & 向右 判定多少行

#define COLUMN_TAR 60 //列判定阈值
#define ROW_TAR 55    //行判定阈值

uint8_t img_line_data[MT9V03X_CSI_H][MT9V03X_CSI_W];
uint8_t img_thres_data[MT9V03X_CSI_H][MT9V03X_CSI_W];
image_t img_raw = DEF_IMAGE(NULL, MT9V03X_CSI_W, MT9V03X_CSI_H);
image_t img_thres = DEF_IMAGE((uint8_t *) img_thres_data, MT9V03X_CSI_W, MT9V03X_CSI_H);
image_t img_line = DEF_IMAGE((uint8_t *) img_line_data, MT9V03X_CSI_W, MT9V03X_CSI_H);

int tempTest;

location_goal Car={0}; //小车状态（位置，目标）存储结构体

bool initHashMapFlag = false; //是否初始化哈希表

const float PI = 3.1415926f;

//边缘线坐标 Xt 上边缘X坐标 Xb 下边缘X坐标 Yl 左边缘Y坐标 Yr 右边缘Y坐标
uint8 Xt = 10, Xb = 35, Yl = 10, Yr = 45;


//-------------------结构体-----------------------//

struct Route_Point {//距离结构体
	int num;//序号
	float dist;//距离
	bool isPointed;//是否已经计算过距离
} HashMap[locate_sz];

//---------------------数组-----------------------//

int originMap[locate_sz][2] = {//坐标原始数据
        0
};

/*
 ***************************************************************
*	函 数 名: Locate_Route
*   功能说明: 路径规划
*	形    参: 无
*	返 回 值: num:路径点序号
 ***************************************************************
 *  入口
 *  int t;//t代表点的个数，只初始化前t个结构体
	scanf("%d",&t);
	initHashMap(t);
    int best,routes;//best代表最优路径，routes代表路径的序号
	if(initHashMapFlag){
		for(int i=0;i<t;++i){
			best=locate_route();
			routes=HashMap[best].num;
			Car.x=originMap[routes][0];
			Car.y=originMap[routes][1];
			printf("%f %f %d\n",xx,yy,routes);
		}
	}
	else printf("Init Failed");
 */
void initHashMap(int true_sz) {//初始化HashMap
	for (int i = 0; i < locate_sz; ++i) {
		HashMap[i].num = i;
		HashMap[i].dist = 0;
		if (i < true_sz)HashMap[i].isPointed = false;
		else HashMap[i].isPointed = true;
	}
	initHashMapFlag = 1;
}

int cmpDist(const void *aa, const void *bb){//给qsort的比较函数（从小到大）
	return (*(struct Route_Point*)aa).dist>(*(struct Route_Point*)bb).dist;
}

int locate_route() {//路径规划
	int num = 0;
	float minDist = 100000;
	for (int i = 0; i < locate_sz; ++i)if (HashMap[i].isPointed == false) {
			float dist = powf(originMap[i][0] - Car.x, 2) + powf(originMap[i][1] - Car.y, 2);
			if (dist < minDist) {minDist = dist;num = i;}
	}
	HashMap[num].isPointed = true;
        tempTest=HashMap[num].num;
	return num;
}
void get_route(){
    int best=locate_route();
    int nextpoint=HashMap[best].num;
    Car.x1=originMap[nextpoint][0];
    Car.y1=originMap[nextpoint][1];
}
void Find_Edge_1(void)
{
    uint8_t mt9v03x_thres_image[MT9V03X_CSI_W][MT9V03X_CSI_H];
    memset(&mt9v03x_thres_image,*img_thres.data,sizeof(img_thres.data));//
    uint8 aspect = 255;   //相邻3 行|列 压缩特征值
    uint8 black_nums = 0; //记录当前扫描行黑点数值
    //上半区域 从中间向上搜索 逐行
    for (uint8 i = 60; i > 60 - ROW_SCOPE; --i)
    {
        black_nums = 0;
        //逐行搜索 每次把搜索点所处列上下点的数据融合
        for (uint8 j = 94 - COLUMN_OFFSET; j < 94 + COLUMN_OFFSET; ++j)
        {
            aspect = mt9v03x_thres_image [i - 1][j] &
                    mt9v03x_thres_image [i][j] & mt9v03x_thres_image[i + 1][j];
            if (aspect == 0)
                black_nums++;
        }
        if (black_nums > COLUMN_TAR)
        {
            Xt = i;
            break;
        }
    }

    //右半区域 从中间向右搜索 逐列
    for (uint8 j = 94; j < 94 + COLUMN_SCOPE; ++j)
    {
        black_nums = 0;
        //逐行搜索 每次把搜索点所处行上下点的数据融合
        for (uint8 i = 60 - ROW_OFFSET; i < 60 + ROW_OFFSET; ++i)
        {
            aspect = mt9v03x_thres_image[i][j - 1] &
                     mt9v03x_thres_image[i][j] & mt9v03x_thres_image[i][j + 1];
            if (aspect == 0)
                black_nums++;
        }
        if (black_nums > ROW_TAR)
        {
            Yr = j;
            break;
        }
    }

    //下半区域 从中间向下搜索 逐行
    for (uint8 i = 60; i < 60 + ROW_SCOPE; ++i)
    {
        black_nums = 0;
        //逐行搜索 每次把搜索点所处列上下点的数据融合
        for (uint8 j = 94 - COLUMN_OFFSET; j < 94 + COLUMN_OFFSET; ++j)
        {
            aspect = mt9v03x_thres_image[i - 1][j] &
                     mt9v03x_thres_image[i][j] & mt9v03x_thres_image[i + 1][j];
            if (aspect == 0)
                black_nums++;
        }
        if (black_nums > COLUMN_TAR)
        {
            Xb = i;
            break;
        }
    }

    //左半区域 从中间向左搜索 逐列
    for (uint8 j = 94; j > 94 - COLUMN_SCOPE; --j)
    {
        black_nums = 0;
        //逐行搜索 每次把搜索点所处行上下点的数据融合
        for (uint8 i = 60 - ROW_OFFSET; i < 60 + ROW_OFFSET; ++i)
        {
            aspect = mt9v03x_thres_image[i][j - 1] &
                     mt9v03x_thres_image[i][j] & mt9v03x_thres_image[i][j + 1];
            if (aspect == 0)
                black_nums++;
        }
        if (black_nums > ROW_TAR)
        {
            Yl = j;
            break;
        }
    }
}

void carlocation_init(){
    initHashMap(12);
    Car.x=1;
    Car.y=0;
	int tmp=locate_route();
    Car.x1=originMap[tmp][0];
    Car.y1=originMap[tmp][1];
    Car.Position_Pointer=0;
    Car.Angel_Target=atan2((Car.x1-Car.x),(Car.y1-Car.y))*180/PI;
    Car.DistanceX=20*(Car.x1-Car.x);
    Car.DistanceY=20*(Car.y1-Car.y);
    //Car.DistanceY=sqrt(Car.DistanceX*Car.DistanceX+Car.DistanceY*Car.DistanceY);
}

void location_entry(){
    while(1){

        }
}

void location_thres_init(void)
{
    rt_thread_t tid;
    //初始化
    //创建寻找坐标线程 优先级设置为8
    tid = rt_thread_create("location", location_entry, RT_NULL, 2048, 31, 100);
    //优先级比主线程高，刚开始就执行让其卡死在此线程，找到坐标后改线程return回收

    //启动寻找坐标线程
    if (RT_NULL != tid)
    {
        rt_thread_startup(tid);
    }
}

/*
//转换坐标
void map_change(int input[][2], int output[][2], int pts_nums, int *pts_label, int yr, int xb) {
    for (int i = 0; i <= pts_nums; ++i) {
        output[i][0] = input[pts_label[i]][0];
        output[i][1] = input[pts_label[i]][1];
    }//重新排序

    for (int i = 0; i <= pts_nums; ++i) {
        output[i][0] = abs(output[i][0] - yr);
        output[i][1] 

= abs(output[i][1] - xb);
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
