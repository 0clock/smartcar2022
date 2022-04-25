/*
 * 路径规划V0.1
 * 根据与原点位置判断计算点位
 * ISSUE
 * 1.无法关联坐标，可能出现大量横向，折线路径
 * 2.定义点数量n无法应用在a[n][2],D[n]中，可能是全局变量的原因
 * 需测试在整体项目中的兼容性
 * TODO
 * 使用点间距离代替与原点距离
 * 在整体项目中测试issue2的可行性
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>//有没有这个库？
const int n=5;//给定值
int a[5][2]={
    1,20,
    3,4,
    15,20,
    7,8,
    1,5
};
typedef struct Dist{//距离结构体
    int num;//序号
    double dist;//距离
}Dist;
struct Dist D[5];
int cmpFunc(const void *aa, const void *bb){//判断函数，现在是从小到大
    return (*(Dist*)aa).dist>(*(Dist*)bb).dist?1:-1;
}

double dis(int aNum){
    double tmpDis= sqrt(pow(a[aNum][0],2)+ pow(a[aNum][1],2));
    return tmpDis;
}

void Distance(){//主函数
    for(int i=0;i<n;++i){
        D[i].dist= dis(i);
        D[i].num=i;
        printf("%d %lf\n",D[i].num,D[i].dist);//输出算好的位置和序号对应关系
    }
    qsort(D,n, sizeof(struct Dist),cmpFunc);//排序
    printf("--顺序点位--\n");
    for(int i=0;i<n;++i){
        printf("(%d,%d)\n",a[D[i].num][0],a[D[i].num][1]);//根据序号输出a(x,y), x=a[D[i].num][0] y=a[D[i].num][1]
    }
}

int main(){
    Distance();
}