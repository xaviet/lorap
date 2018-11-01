/*
 * hw_config.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef HW_CONFIG_H_
#define HW_CONFIG_H_

#include "stm32f10x.h"

//|--------------|
//|  0x8003ffff  |
//|              |  upgrade prog (120K Bytes)
//|  0x08022000  |
//|--------------|
//|  0x08021fff  |
//|              |  app prog (120K Bytes)
//|  0x08004000  |
//|--------------|
//|  0x08003fff  |
//|              |  env data (2K Bytes)
//|  0x08003800  |
//|--------------|
//|  0x080037ff  |
//|              |  bootloader (14K Bytes)
//|  0x08000000  |
//----------------
#define FLASH_START_ADDR ((u32)0x08000000)
#define FLASH_SECTOR_NUM ((u32)0x80)
#define FLASH_SECTOR_SIZE ((u32)0x800)
#define FLASH_UPGRADE_PROG_ADDRESS ((u32)0x08022000)
#define FLASH_APP_PROG_ADDRESS ((u32)0x08004000)
#define FLASH_ENV_DATA_ADDRESS ((u32)0x08003800)

#define FLASH_SECTOR_TO_SIZE(sector) ((sector) * FLASH_SECTOR_SIZE)
#define FLASH_SIZE_TO_SECTOR(size) ((size) / FLASH_SECTOR_SIZE)
#define FLASH_SECTOR_TO_ADDR(sector) (FLASH_START_ADDR + FLASH_SECTOR_TO_SIZE(sector))
#define FLASH_ADDR_TO_SECTOR(addr) (FLASH_SIZE_TO_SECTOR((FLASH_START_ADDR ^ (addr))))

#define FLASH_END_ADDR FLASH_SECTOR_TO_ADDR(FLASH_SECTOR_NUM)

#define FLASH_PROG_SIZE (FLASH_END_ADDR - FLASH_UPGRADE_PROG_ADDRESS)
#define FLASH_ENV_SIZE (FLASH_APP_PROG_ADDRESS - FLASH_ENV_DATA_ADDRESS)

#define FLASH_UPGRADE_PROG_SECTOR FLASH_ADDR_TO_SECTOR(FLASH_UPGRADE_PROG_ADDRESS)
#define FLASH_APP_PROG_SECTOR FLASH_ADDR_TO_SECTOR(FLASH_APP_PROG_ADDRESS)
#define FLASH_ENV_DATA_SECTOR FLASH_ADDR_TO_SECTOR(FLASH_ENV_DATA_ADDRESS)

#define USART_DEBUG USART1
#define USART_GPIO GPIOA
#define USART_TX GPIO_Pin_9
#define USART_RX GPIO_Pin_10
#define USART_SPEED 115200
#define USART_IRQ USART1_IRQn

#define LED_PIN GPIO_Pin_8
#define LED_GPIO GPIOA

#endif /* HW_CONFIG_H_ */