/*
 * drv_gpio.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "hw_config.h"
#include "drv_gpio.h"

void init_gpio()
{
  gpio_init(LED_PIN, LED_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
//  gpio_init(BUTTON_PIN, BUTTON_GPIO, GPIO_Speed_2MHz, GPIO_Mode_IPU);
}

void gpio_init(u16 pin, GPIO_TypeDef* gpio, u32 speed, u32 mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = pin;
  GPIO_InitStructure.GPIO_Speed = speed;
  GPIO_InitStructure.GPIO_Mode = mode;
  GPIO_Init(gpio, &GPIO_InitStructure);
}

void gpio_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, vu8 flag)
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
  gpio_set(LED_GPIO, LED_PIN, !flag);
}

//__IO u8 button_get()
//{
//  return(gpio_get(BUTTON_GPIO, BUTTON_PIN));
//}
