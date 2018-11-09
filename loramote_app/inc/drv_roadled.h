/*
 * drv_roadled.h
 *
 *  Created on: Nov 9, 2018
 *      Author: mose
 */

#ifndef DRV_ROADLED_H_
#define DRV_ROADLED_H_

#include "stm32f10x.h"

#define ROADLED_PIN GPIO_Pin_8
#define ROADLED_GPIO GPIOB

#define LEDSENSOR_PIN GPIO_Pin_7
#define LEDSENSOR_GPIO GPIOB

void init_roadled();

#endif /* DRV_ROADLED_H_ */
