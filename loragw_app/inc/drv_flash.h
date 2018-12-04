/*
 * drv_flash.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef DRV_FLASH_H_
#define DRV_FLASH_H_

#include "stm32f10x.h"

void init_flash();
u8 flash_erase(u32 sector, u32 count);
u32 flash_write(u32 sector, u8* buffer, u32 length);
u32 flash_read(u32 sector, u8* buffer, u32 length);

#endif /* DRV_FLASH_H_ */
