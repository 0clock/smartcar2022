#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
  GPIO_InitStruct.Pin =GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);

}





