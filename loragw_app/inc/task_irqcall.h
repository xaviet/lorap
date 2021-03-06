/*
 * task_irqcall.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_IRQCALL_H_
#define TASK_IRQCALL_H_

#include "global.h"

void irqcall_init();
void irqcall_run();
void irqCall_default();
void sx1278_dio0_irq();
void w5500_int_irq();
void TIM1_irq();
void TIM2_irq();
void USART1_rx();
void statesMachineJump();
u8 config_msg_format(struct SconfigMsg* msg, u8 vCrc8, u8 idCheck);
u8 data_msg_format(struct SworkDataMsg* msg);

#endif /* TASK_IRQCALL_H_ */
