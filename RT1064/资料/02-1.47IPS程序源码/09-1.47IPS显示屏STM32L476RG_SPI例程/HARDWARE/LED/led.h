#ifndef __LED_H
#define __LED_H

#include "sys.h"



#define LED_ON    HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_RESET)
#define LED_OFF   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12, GPIO_PIN_SET)

void LED_Init(void);


#endif




