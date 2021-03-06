/*
 * task.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "task.h"
#include "string.h"

void init_task()
{
  // typedef enum taskList
  // {run_mark, timer(startup delay), time_interval, task_point}
  TASKCOMPSREG(taskComps)
  for(int i = 0; i < EtaskMax; i++)
  {
    memcpy(&(globalV.taskComps[i]), &(taskComps[i]), sizeof(struct StaskComponents));
    if(globalV.taskComps[i].task_init_hook)
    {
      globalV.taskComps[i].task_init_hook();
    }
  }
}

void task_timer()
{
  for(int i=0; i < EtaskMax; i++)
  {
    if(globalV.taskComps[i].timer)
    {
      globalV.taskComps[i].timer--;
    }
    else
    {
      globalV.taskComps[i].timer = globalV.taskComps[i].itvTime;
      globalV.taskComps[i].run = TRUE;
    }
  }

}

void task_run()
{
  for(int i=0; i < EtaskMax; i++)
  {
    if(globalV.taskComps[i].run)
    {
      if(globalV.taskComps[i].task_hook)
      {
        globalV.taskComps[i].task_hook();
      }
      if(globalV.taskComps[i].itvTime > 0)
      {
        globalV.taskComps[i].run = FALSE;
      }
    }
  }
}
