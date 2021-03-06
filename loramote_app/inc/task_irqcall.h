/*
 * task_irqcall.h
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#ifndef TASK_IRQCALL_H_
#define TASK_IRQCALL_H_

#include "stm32f10x.h"
#include "global.h"

#define TASK_IRQCALL_STARTUPDELAY 2
#define TASK_IRQCALL_INTERVALTIME 0

void irqcall_init();
void irqcall_run();
void irqCall_default();
void sx1278_dio0_irq();
void TIM1_irq();
void TIM2_irq();
void USART1_rx();
void statesMachineJump();
u8 config_msg_format(struct SmsgHead* msg, u8 vCrc8, u8 idCheck);

#endif /* TASK_IRQCALL_H_ */
