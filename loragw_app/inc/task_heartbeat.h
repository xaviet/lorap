/*
 * task_heartbeat.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_HEARTBEAT_H_
#define TASK_HEARTBEAT_H_

#include "stm32f10x.h"

#define HEARTBEAT_RX_TIMEOUT 8192
#define HEARTBEAT_INTERVAL 65536

#define LEDBLINKDIVDONE 512
#define LEDBLINKDIVTIMEOUT 1024

void heartbeat_init();
void heartbeat_run();
void heartbeatStates_delay();
void heartbeatStates_rxDone();
void heartbeatStates_rxing();
void heartbeatStates_tx();
void heartbeatStates_default();

#endif /* TASK_HEARTBEAT_H_ */
