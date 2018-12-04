/*
 * task.h
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#ifndef TASK_H_
#define TASK_H_

#include "global.h"
#include "task_irqcall.h"
#include "task_forward.h"
#include "task_heartbeat.h"
#include "task_reboot.h"
#include "task_upgrade.h"

// typedef enum taskList
// {run_mark, timer(startup delay), time_interval, task_point}
#define TASKCOMPSREG(taskComps) \
struct StaskComponents taskComps[EtaskMax] = \
{ \
  {FALSE, HEARTBEAT_RX_TIMEOUT, TASKINTERVALTIME, heartbeat_init, heartbeat_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, irqcall_init, irqcall_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, forward_init, forward_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, reboot_init, reboot_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, upgrade_init, upgrade_run}, \
};
typedef enum EtaskList
{
  EheartBeat = 0,
  EirqCall,
  Eforward,
  Ereboot,
  Eupgrade,
  EtaskMax, // max < 16
}taskList;

void init_task();
void task_timer();
void task_run();

#endif /* TASK_H_ */
