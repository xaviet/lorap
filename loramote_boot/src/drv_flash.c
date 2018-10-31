/*
 * drv_flash.c
 *
 *  Created on: Sep 27, 2018
 *      Author: pv
 */

#include "drv_flash.h"
#include "drv_usart.h"
#include "string.h"

void flash_init()
{

}

u8 flash_erase(u32 sector, u32 count)
{
  for(u32 i = 0; i < count; ++i)
  {
    if(FLASH_ErasePage(FLASH_SECTOR_ADDR(sector) + i * FLASH_SECTOR_SIZE) != FLASH_COMPLETE)
    {
      usart_send_string("\r\nflash_erase fault\r\n");
      return(FALSE);
    }
  }
  return(TRUE);
}

u32 flash_write(u32 sector, u8* buffer, u32 length)
{
  u16 data = 0;
  for(u32 i = 0; i < length; i += 2)
  {
    data = (*(buffer + i + 1) << 8) + (*(buffer + i));
    if(FLASH_ProgramHalfWord((uint32_t)(FLASH_SECTOR_ADDR(sector) + i), data) != FLASH_COMPLETE)
    {
      usart_send_string("\r\nflash_write fault\r\n");
      return(FALSE);
    }
  }
  return(length);
}

u32 flash_read(u32 sector, u8* buffer, u32 length)
{
  memcpy(buffer, (void*)FLASH_SECTOR_ADDR(sector), length);
  return(length);
}

u8 flash_erase_disable_irq(u32 sector, u32 count)
{
  __disable_irq();
  FLASH_Unlock();
  u8 rt = flash_erase(sector, count);
  FLASH_Lock();
  __enable_irq();
  return(rt);
}

u32 flash_write_disable_irq(u32 sector, u8* buffer, u32 length)
{
  __disable_irq();
  FLASH_Unlock();
  u32 rt = flash_write(sector, buffer, length);
  FLASH_Lock();
  __enable_irq();
  return(rt);
}
