/*
 * drv_iwdg.h
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#ifndef DRV_IWDG_H_
#define DRV_IWDG_H_

#include "stm32f10x.h"

#define IWDGRELOAD 4095 //设置 IWDG(12 bits) 重装载值 6.552s; actual = (4095 / 625)

void init_iwdg();
void iwdg_feed();

#endif /* DRV_IWDG_H_ */
