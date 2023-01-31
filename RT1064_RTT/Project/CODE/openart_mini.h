#ifndef _openartart_mini_h
#define _openartart_mini_h

#include "headfile.h"


typedef struct openart_param_t {
    int8_t rx_array[5];     //存储串口接受数据
    int8_t openart_result;  //结果

    enum {
        NONE, ANIMAL, FRUIT, TRANSPORTATION
    } fa_type;              // 靶牌分类结果


} openart_param_t;

enum picture_type{
    apple,
    banana,
    bus,
    car,
    cat,
    cow,
    dog,
    durian,
    grape,
    horse,
    orange,
    pig,
    plane,
    ship,
    trains
};

enum picture_big_type{
    fruit,
    animal,
    vehicle
};

enum openart_mode{
    get_map,
    get_picture
};

extern enum picture_type picture_type;
extern enum picture_big_type pictureBigType;
extern enum openart_mode openartMode;
extern bool havePicture;
extern bool isDetection;

extern int32 picture_location[2];
extern uint8 edge_rotation[2];

extern openart_param_t openart;

void openart_send(void);

void openart_mini(void);

void check_openart(void);

#endif

