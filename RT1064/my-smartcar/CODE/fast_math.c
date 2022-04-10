//
// Created by Jay on 2022/4/9.
//

#include "fast_math.h"


/*
***************************************************************
*�� �� ��: myinvSqrt(float x)
*����˵��: ���ټ���x�����Ŀ���
*��    ��: �������ֵ
*�� �� ֵ: ���
***************************************************************
*/
float myinvSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}
/*
***************************************************************
*�� �� ��: myatan(float v)
*����˵��: ���ټ��㷴����
*��    ��: �������ֵ
*�� �� ֵ: ���
***************************************************************
*/
float myatan(float v)
{
    float v2 = v*v;
    return (v*(1.6867629106f + v2*0.4378497304f)/(1.6867633134f + v2));
}