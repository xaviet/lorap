/*
 * drv_exti.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef DRV_EXTI_H_
#define DRV_EXTI_H_

#include "stm32f10x.h"

extern void (*sx1278_dio0_IRQ_call)();
extern void (*w5500_int_IRQ_call)();

void init_exti();
void exti_init(u16 spiN, GPIO_TypeDef* gpio, u32 extiLine, u8 portSource, u8 pinSource, u8 channel, EXTITrigger_TypeDef trigger);
void EXTI0_IRQHandler();
void EXTI4_IRQHandler();

#endif /* DRV_EXTI_H_ */
