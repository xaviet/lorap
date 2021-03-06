/*
 * drv_gpio.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "drv_gpio.h"

void init_gpio()
{
  led_init(LED_PIN, LED_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
}

void led_init(u16 pin, GPIO_TypeDef* gpio, u32 speed, u32 mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
   GPIO_InitStructure.GPIO_Pin = pin;
   GPIO_InitStructure.GPIO_Speed = speed;
   GPIO_InitStructure.GPIO_Mode = mode;
   GPIO_Init(gpio, &GPIO_InitStructure);
}

void gpio_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u8 flag)
{
  if(flag)
   {
     GPIO_SetBits(GPIOx, GPIO_Pin);
   }
   else
   {
     GPIO_ResetBits(GPIOx, GPIO_Pin);
   }
}

__IO u8 gpio_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  return(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin));
}

void led_set(vu8 flag)
{
  // on by high
  gpio_set(LED_GPIO, LED_PIN, !flag);
}
