/*
 * drv_ds18b20.c
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#include "drv_ds18b20.h"
#include "drv_gpio.h"
#include "drv_usart.h"
#include "config.h"
#include "tools_lib.h"
#include "global.h"

void init_ds18b20()
{
  gpio_init(DS18B20_PIN, DS18B20_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_OD);
  ds18b20_set(ON);
  while(!ds18b20_get());
  if(ds18b20_reset())
  {
    usart_send_string("ds18b20_reset done. ID: ");
    u8 data[COMMONBUFFERLENGTH] = {0};
    ds18b20_write_byte(0x33);
    ds18b20_read_nByte(data, 8);
    usart_send_u8_array(data, 8);
    ds18b20_get_temp(data);
    usart_send_string("eTemp: ");
    usart_send_u8_array(data, 9);
  }
  else
  {
    usart_send_string("ds18b20_reset error\r\n");
  }
}

void ds18b20_set(u8 flag)
{
  if(flag)
   {
     GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);
   }
   else
   {
     GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);
   }
}

u8 ds18b20_get()
{
  return(GPIO_ReadInputDataBit(DS18B20_GPIO, DS18B20_PIN));
}

u8 ds18b20_reset()
{
  u8 rt = ON;
  u32 timeStamp = 0;
  ds18b20_set(OFF);
  delay_1us(500);
  ds18b20_set(ON);
  delay_1us(15);
  timeStamp = globalV.msTicket;
  while(ds18b20_get())
  {
    if(delta_ticket(timeStamp, globalV.msTicket) > ONEWIRE_TIMEOUT)
    {
      usart_debug("ds18b20_reset no_ack");
      rt = OFF;
      break;
    }
  }
  delay_1us(500);
  return(rt);
}

u8 ds18b20_read_bit()
{
  u8 rt = 0;
  ds18b20_set(OFF);
  delay_1us(5);
  ds18b20_set(ON);
  delay_1us(5);
  rt = ds18b20_get();
  delay_1us(55);
  return(rt);
}

void ds18b20_write_bit(u8 d)
{
  ds18b20_set(OFF);
  delay_1us(15);
  if(d)
  {
    ds18b20_set(ON);
    delay_1us(45);
  }
  else
  {
    delay_1us(45);
  }
  ds18b20_set(ON);
  delay_1us(5);
}

u8 ds18b20_read_byte()
{
  u8 rt = 0;
  for(u8 i = 0; i < 8; i++)
  {
    rt = rt + (ds18b20_read_bit() << i);
  }
  return(rt);
}

void ds18b20_write_byte(u8 d)
{
  for(u8 i = 0; i < 8; i++)
  {
    ds18b20_write_bit((d >> i) & 0x01);
  }
}

void ds18b20_read_nByte(u8* data, u8 length)
{
  while(length--)
  {
    *(data++) = ds18b20_read_byte();
  }
}

void ds18b20_write_nByte(u8* data, u8 length)
{
  while(length--)
  {
    ds18b20_write_byte(*(data++));
  }
}

void ds18b20_get_temp(u8* data)
{
  __disable_irq();
  ds18b20_reset();
  ds18b20_write_byte(0xcc);
  ds18b20_write_byte(0x44);
  __enable_irq();
  delay_1us(750000);
  __disable_irq();
  ds18b20_reset();
  ds18b20_write_byte(0xcc);
  ds18b20_write_byte(0xbe);
  ds18b20_read_nByte(data, 9);
  __enable_irq();
}
