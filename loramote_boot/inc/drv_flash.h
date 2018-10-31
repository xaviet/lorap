/*
 * drv_flash.h
 *
 *  Created on: Sep 27, 2018
 *      Author: pv
 */

#ifndef DRV_FLASH_H_
#define DRV_FLASH_H_

#include "config.h"
#include "stm32f10x.h"

#ifdef STM32F103RET6
#define FLASH_SECTOR_NUM    256
#define FLASH_SECTOR_SIZE   2048
#endif

#ifdef STM32F103RCT6
#define FLASH_SECTOR_NUM    128
#define FLASH_SECTOR_SIZE   2048
#endif

#ifdef STM32F103C8T6
#define FLASH_SECTOR_NUM    64
#define FLASH_SECTOR_SIZE   1024
#endif

#define FLASH_START_ADDR        ((u32)0x08000000)
#define FLASH_END_ADDR          ((u32)(FLASH_START_ADDR + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))
#define FLASH_SECTOR_ADDR(num)  ((u32)(FLASH_START_ADDR + (num) * FLASH_SECTOR_SIZE))
#define FLASH_ENV_VALUE_SECTOR        ((u32)FLASH_SECTOR_NUM - 1)

void flash_init();
u8 flash_erase(u32 sector, u32 count);
u32 flash_write(u32 sector, u8* buffer, u32 length);
u32 flash_read(u32 sector, u8* buffer, u32 length);
u8 flash_erase_disable_irq(u32 sector, u32 count);
u32 flash_write_disable_irq(u32 sector, u8* buffer, u32 length);


#endif /* DRV_FLASH_H_ */
