/*
 * task_forward.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_FORWARD_H_
#define TASK_FORWARD_H_

#include "stm32f10x.h"

void forward_init();
void forward_run();
void forward_upStreamRxDone();
void forward_upStreamRxing();
void forward_upStreamRx();
void forward_downStreamTxDone();
void forward_downStreamTxing();
void forward_downStreamRx();
void forward_loginReplyTxDone();
void forward_loginReplyTxing();
void forward_loginReplyRx();
void forward_loginRequestRx();
void forward_rxing();
void forward_loginChannelListen();
void forward_default();

#endif /* TASK_FORWARD_H_ */
