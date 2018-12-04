/*
 * drv_gpio.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef DRV_GPIO_H_
#define DRV_GPIO_H_

#include "global.h"
#include "tools_lib.h"

void init_gpio();
void led_init(u16 pin, GPIO_TypeDef* gpio, u32 speed, u32 mode);
void gpio_set(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, u8 flag);
__IO u8 gpio_get(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void led_set(vu8 flag);

#endif /* DRV_GPIO_H_ */
