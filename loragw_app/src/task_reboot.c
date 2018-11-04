/*
 * task_reboot.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "task_reboot.h"
#include "drv_gpio.h"
#include "hw_config.h"
#include "config.h"
#include "global.h"
#include "drv_flash.h"
#include "tools_lib.h"
void reboot_init()
{
  gpio_set(BUTTON_GPIO, BUTTON_PIN, ON);
  globalV.rebootPressedTime = 0;
}

void reboot_run()
{
  if(!gpio_get(BUTTON_GPIO, BUTTON_PIN))
   {
     if(globalV.rebootPressedTime == 0)
     {
       globalV.rebootPressedTime = globalV.upSeconds;
     }
     else if(globalV.upSeconds - globalV.rebootPressedTime >= PRESSTIMEINIT)
     {
       globalV.ledBlinkDiv = LEDBLINKDIVINIT;
     }
     else if(globalV.upSeconds - globalV.rebootPressedTime >= PRESSTIMEREBOOT)
     {
       globalV.ledBlinkDiv = LEDBLINKDIVREBOOT;
     }
   }
   else
   {
     if(globalV.ledBlinkDiv == LEDBLINKDIVREBOOT)
     {
       NVIC_SystemReset();
     }
     else if(globalV.ledBlinkDiv == LEDBLINKDIVINIT)
     {
       flash_erase(FLASH_ENV_DATA_SECTOR, 1);
       struct SflashEnvValue flashEnvValue;
       flashEnvValue.upgradeFlag = FALSE;
       flashEnvValue.crc8 = crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1);
       flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&flashEnvValue, 1);
       NVIC_SystemReset();
     }
     globalV.rebootPressedTime = 0;
     globalV.ledBlinkDiv = LEDBLINKDIV;
   }
}
