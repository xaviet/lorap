/*
 * main.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "main.h"
#include "drv_usart.h"
#include "drv_flash.h"
#include "drv_gpio.h"

int main(void)
{
  init();
  loop();
}

void init()
{
  // 外设使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			 RCC_APB2Periph_GPIOB |
//			 RCC_APB2Periph_GPIOC |
			 RCC_APB2Periph_USART1
			 , ENABLE);

  init_usart();
  init_gpio();
  init_flash();
  init_global();

  struct SflashEnvValue flashEnvValue;
  flash_read(FLASH_ENV_DATA_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
  usart_send_string("\r\n\tComplie: ");
  usart_send_string(__DATE__);
  usart_send_string(" ");
  usart_send_string(__TIME__);
  usart_send_string("\r\n********************************************************************************\r\n\tLoRaMote boot app ver: ");
  u32 upgradeSectors = (flashEnvValue.upgradeDataSize / FLASH_SECTOR_SIZE) +
		      ((flashEnvValue.upgradeDataSize % FLASH_SECTOR_SIZE > 0)?1: 0);
  usart_send_u8(flashEnvValue.ver);
  usart_send_string(" upgrade: ");
  usart_send_u8(flashEnvValue.upgradeFlag);
  usart_send_string(" size: ");
  usart_send_u32(flashEnvValue.upgradeDataSize);
//  usart_send_string(" sectors: ");
//  usart_send_u32(upgradeSectors);
  usart_send_string("\r\n");
//  flashEnvValue.upgradeFlag = FALSE;
  if((flashEnvValue.upgradeFlag == TRUE) &&
      (flashEnvValue.crc8 == crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1)))
  {
    for(int i = 0; i < upgradeSectors; i++)
    {
//      usart_send_u32(i);
//      usart_send_u32(FLASH_APP_PROG_SECTOR + i);
//      usart_send_u32(FLASH_UPGRADE_PROG_SECTOR + i);
//      usart_send_string("\r\n");
      flash_erase(FLASH_APP_PROG_SECTOR + i, 1);
      flash_write(FLASH_APP_PROG_SECTOR + i, (u8*)FLASH_SECTOR_TO_ADDR(FLASH_UPGRADE_PROG_SECTOR + i), FLASH_SECTOR_SIZE);
//      u8 d[FLASH_SECTOR_SIZE];
//      flash_read(FLASH_UPGRADE_PROG_SECTOR + i, (u8*)&d, FLASH_SECTOR_SIZE);
//      usart_send_u8_array((u8*)&d, FLASH_SECTOR_SIZE);
//      flash_read(FLASH_APP_PROG_SECTOR + i, (u8*)&d, FLASH_SECTOR_SIZE);
//      usart_send_u8_array((u8*)&d, FLASH_SECTOR_SIZE);
    }
    flashEnvValue.upgradeFlag = FALSE;
    flashEnvValue.crc8 = crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1);
    flash_erase(FLASH_ENV_DATA_SECTOR, 1);
    flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
    usart_send_string("\tupgrade OK\r\n");
  }
}

void loop()
{
//  while(TRUE)
//  {
//    led_set(ON);
//    delay_1us(0x8000);
//    led_set(OFF);
//    delay_1us(0x8000);
//  }
  // jump app
  jumper();
}

void jumper()
{
  if(((*(vu32*)FLASH_APP_PROG_ADDRESS) & 0x2FFE0000) == 0x20000000)
  {
    pFun jump = (pFun)*(vu32*)(FLASH_APP_PROG_ADDRESS + 4);
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, FLASH_APP_PROG_ADDRESS);
    __set_MSP(*(vu32*)FLASH_APP_PROG_ADDRESS);
//    SCB->VTOR = (u32)FLASH_APP_PROG_ADDRESS;
//    asm volatile("msr msp, %0"::"g"(*(volatile u32*) FLASH_APP_PROG_ADDRESS));
    jump();
  }
}
