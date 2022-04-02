#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_ON  GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_15)

void LED_Init(void);


#endif




