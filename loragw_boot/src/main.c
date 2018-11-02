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
//			 RCC_APB2Periph_GPIOB |
			 RCC_APB2Periph_USART1
			 , ENABLE);

  init_gpio();
  init_usart();
  init_flash();
  init_global();

  struct SflashEnvValue flashEnvValue;
  flash_read(FLASH_ENV_DATA_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
  usart_send_string("\r\n\tBoot ver: ");
  usart_send_u8(flashEnvValue.ver);
  usart_send_string(" upgrade: ");
  usart_send_u8(flashEnvValue.upgradeFlag);
  usart_send_string("\r\n");
  if((flashEnvValue.upgradeFlag == TRUE) &&
      (flashEnvValue.crc8 == crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1)))
  {
    for(int i = 0; i < FLASH_SIZE_TO_SECTOR(flashEnvValue.upgradeDataSize); i++)
    {
      flash_erase(FLASH_APP_PROG_SECTOR + 1, 1);
      flash_write(FLASH_APP_PROG_SECTOR + i, (u8*)FLASH_SECTOR_TO_ADDR(FLASH_UPGRADE_PROG_SECTOR + i), FLASH_SECTOR_SIZE);
    }
    flashEnvValue.upgradeFlag = FALSE;
    flashEnvValue.crc8 = crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1);
    flash_erase(FLASH_ENV_DATA_SECTOR, 1);
    flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
    usart_send_string("\tupgrading OK\r\n");
  }

  // jump app
  jumper();
}

void loop()
{
  while(TRUE)
  {
    led_set(ON);
    delay_1us(0x8000);
    led_set(OFF);
    delay_1us(0x8000);
  }
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
