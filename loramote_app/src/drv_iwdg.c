/*
 * drv_iwdg.c
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#include "drv_iwdg.h"

void init_iwdg()
{
  // Tout = (Prescaler * Reload) / LSI, LSI = 40000hz
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  //使能对寄存器 IWDG 写操作
  IWDG_SetPrescaler(IWDG_Prescaler_64);  //设置 IWDG 预分频值:设置 IWDG 预分频值
  IWDG_SetReload(IWDGRELOAD);  //设置 IWDG(12 bits) 重装载值 6.552s; actual = (4095 / 625)
  IWDG_ReloadCounter(); //按照 IWDG 重装载寄存器的值重装载 IWDG 计数器
  IWDG_Enable();  //使能 IWDG
}

void iwdg_feed()
{
  IWDG_ReloadCounter();//reload
}
