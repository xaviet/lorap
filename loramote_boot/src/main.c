/**
  ******************************************************************************
  * @file    main.c
  * @author  Mose
  * @version V1.0
  * @date    01-December-2018
  * @brief   Default main function.
  ******************************************************************************
*/

#include "main.h"

int main(void)
{
  usart_init();
  usart_send_string("\r\nbootloader init Done\r\n");
  flash_init();

  struct SflashEnvValue flashEnvValue;
  flash_read(FLASH_ENV_VALUE_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
  if((flashEnvValue.upgrade_flag == 0x01) &&
    (flashEnvValue.crc8 == crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1)))
  {
    u32 appSector = ((u32)APP_PROG_ADDRESS ^ NVIC_VectTab_FLASH) / FLASH_SECTOR_SIZE;
    u32 dataSector = ((u32)UPGRADE_DATA_ADDRESS ^ NVIC_VectTab_FLASH) / FLASH_SECTOR_SIZE;
    usart_send_string("\r\nupgrade: 00/26");
    for(int i = 0; i < DATA_SIZE / FLASH_SECTOR_SIZE; i++)
    {
      flash_erase_disable_irq(appSector + i, 1);
//      delay_1us(10000);
      flash_write_disable_irq(appSector + i, (u8*)FLASH_SECTOR_ADDR(dataSector + i), FLASH_SECTOR_SIZE);
//      delay_1us(10000);
      usart_send_string("\b\b\b\b\b");
      u8 disp = i / 16;
      usart_send_byte(USARTX, disp + 0x30);
      disp = i % 16;
      usart_send_byte(USARTX, disp + 0x30);
      usart_send_string("/26");
    }
    flashEnvValue.upgrade_flag = 0x00;
    flashEnvValue.crc8 = crc8((u8*)&flashEnvValue, sizeof(struct SflashEnvValue) - 1);
    flash_erase_disable_irq(FLASH_ENV_VALUE_SECTOR, 1);
    flash_write_disable_irq(FLASH_ENV_VALUE_SECTOR, (u8*)&flashEnvValue, sizeof(struct SflashEnvValue));
    usart_send_string("\r\nupgrade OK!\r\n");
  }

  // jump iap
  jumper();
}

void jumper(void)
{
  if(((*(vu32*)APP_PROG_ADDRESS) & 0x2FFE0000) == 0x20000000)
  {
    usart_send_string("\r\njump to app\r\n");
    pFun jump = (pFun)*(volatile u32*)(APP_PROG_ADDRESS + 4);
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, (u32)APP_PROG_ADDRESS);
    __set_MSP(*(volatile u32*)APP_PROG_ADDRESS);
  //  SCB->VTOR = (u32)IAP_ADDRESS;
  //  asm volatile("msr msp, %0"::"g"(*(volatile u32*) IAP_ADDRESS));
    jump();
  }
  else
  {
    usart_send_string("\r\njump fault\r\n");
  }
}

u8 crc8(u8* chars, u8 length)
{
  u8 rt = 0x00;
  u8 i;
  while(length--)
  {
    rt = (u8)(((u32)rt ^ (*chars++)) & 0xff);
    for(i = 8; i > 0; --i)
    {
      if(rt & 0x80)
      {
        rt = (u8)((((u32)rt << 1) ^ CRC8POLY) & 0xff);
      }
      else
      {
        rt = (u8)(((u32)rt << 1) & 0xff);
      }
    }
  }
  return(rt);
}

void delay_1us(u32 n)
{
  u32 i = 8 * n;
  while(i--);
}
