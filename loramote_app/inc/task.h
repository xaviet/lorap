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
#include "task_sensor.h"

// typedef enum taskList
// {run_mark, timer(startup delay), time_interval, task_point}
#define TASKCOMPSREG(taskComps) \
struct StaskComponents taskComps[EtaskMax] = \
{ \
  {FALSE, TASK_IRQCALL_STARTUPDELAY, TASK_IRQCALL_INTERVALTIME, irqcall_init, irqcall_run}, \
  {FALSE, TASK_FORWARD_STARTUPDELAY, TASK_FORWARD_INTERVALTIME, forward_init, forward_run}, \
  {FALSE, TASK_SENSOR_STARTUPDELAY, TASK_SENSOR_INTERVALTIME, sensor_init, sensor_run}, \
};
typedef enum EtaskList
{
  EirqCall = 0,
  Eforward,
  Esensor,
  EtaskMax, // max < 16
}taskList;

void init_task();
void task_timer();
void task_run();

#endif /* TASK_H_ */
