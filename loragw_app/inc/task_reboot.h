/*
 * task_reboot.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_REBOOT_H_
#define TASK_REBOOT_H_

#include "stm32f10x.h"

#define LEDBLINKDIVREBOOT 256
#define LEDBLINKDIVINIT 64
#define PRESSTIMEREBOOT 2
#define PRESSTIMEINIT 8

void reboot_init();
void reboot_run();


#endif /* TASK_REBOOT_H_ */
