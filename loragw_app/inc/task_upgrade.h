/*
 * task_upgrade.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_UPGRADE_H_
#define TASK_UPGRADE_H_

#include "stm32f10x.h"

#define FLAG_WAITTING 0x00
#define FLAG_TRANSMITTING 0x01
#define FLAG_COMPLETE 0x02
#define FLAG_ERROR 0xff

#define UPGRADE_OVERTIME 8
#define UPGRADE_REPEAT 8
#define UPGRADE_DATA_SIZE 64

void upgrade_init();
void upgrade_run();
void upgradeGotDataSave();
void upgradeGotData();
void upgradeRunning();
void upgradeReplyTx();
u8 upgradeRequestCheck();
#endif /* TASK_UPGRADE_H_ */
