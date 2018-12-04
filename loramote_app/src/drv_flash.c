/*
 * drv_flash.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "config.h"
#include "hw_config.h"
#include "drv_flash.h"
#include "string.h"

void init_flash()
{

}

u8 flash_erase(u32 sector, u32 count)
{
  __disable_irq();
  FLASH_Unlock();
  u8 rt = TRUE;
  for(u32 i = 0; i < count; ++i)
  {
    if(FLASH_ErasePage(FLASH_SECTOR_TO_ADDR(sector) + i * FLASH_SECTOR_SIZE) != FLASH_COMPLETE)
    {
      rt = FALSE;
      break;
    }
  }
  FLASH_Lock();
  __enable_irq();
  return(rt);
}

u32 flash_write(u32 sector, u8* buffer, u32 length)
{
  __disable_irq();
  FLASH_Unlock();
  u8 rt = TRUE;
  u16 data = 0;
  vu32 i = 0;
  for(i = 0; i < length; i += 2)
  {
    data = (*(buffer + i + 1) << 8) + (*(buffer + i));
    if(FLASH_ProgramHalfWord((FLASH_SECTOR_TO_ADDR(sector) + i), data) != FLASH_COMPLETE)
    {
      break;
    }
  }
  rt = (i >= length)? length: FALSE;
  FLASH_Lock();
  __enable_irq();
  return(rt);
}

u32 flash_read(u32 sector, u8* buffer, u32 length)
{
  memcpy(buffer, (void*)FLASH_SECTOR_TO_ADDR(sector), length);
  return(length);
}

