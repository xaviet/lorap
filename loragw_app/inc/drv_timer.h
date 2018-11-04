/*
 * drv_timer.h
 *
 *  Created on: Nov 2, 2018
 *      Author: mose
 */

#ifndef DRV_TIMER_H_
#define DRV_TIMER_H_

#include "stm32f10x.h"

void (*TIM1_IRQ_call)();
void (*TIM2_IRQ_call)();

void init_timer();
void timer_init(TIM_TypeDef* timN, u16 period, u16 prescaler, u16 clockDivision, u16 countMode, u8 repetition, u8 irqChannel);
void TIM1_UP_IRQHandler();
void TIM2_IRQHandler();

#endif /* DRV_TIMER_H_ */
