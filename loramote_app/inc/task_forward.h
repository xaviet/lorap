/*
 * task_forward.h
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#ifndef TASK_FORWARD_H_
#define TASK_FORWARD_H_

#define TASK_FORWARD_STARTUPDELAY 2
#define TASK_FORWARD_INTERVALTIME 0

extern void (*sensor_process)();

void forward_init();
void forward_run();
void forward_upStreamMsgTxDone();
void forward_upStreamMsgTxing();
void forward_upStreamMsgTx();
void forward_sensorProcess();
void forward_downStreamMsgRxDone();
void forward_downStreamMsgRxing();
void forward_downStreamMsgRx();
void forward_setDownStreamChannel();
void forward_loginMsgRxDone();
void forward_loginMsgRxing();
void forward_loginMsgRx();
void forward_EloginMsgTxDelay();
void forward_loginMsgTxDone();
void forward_loginMsgTxing();
void forward_loginMsgTx();
void forward_default();

#endif /* TASK_FORWARD_H_ */
