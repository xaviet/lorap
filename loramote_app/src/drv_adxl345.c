/*
 * drv_adxl345.c
 *
 *  Created on: Nov 21, 2018
 *      Author: mose
 */

#include "hw_config.h"
#include "drv_adxl345.h"
#include "drv_spi.h"
#include "drv_gpio.h"
#include "drv_usart.h"
#include "tools_lib.h"
#include "string.h"

void init_adxl345()
{
  gpio_set(ADXL345_GPIO, ADXL345_NSS, HIGH);
  usart_send_string("\tadxl345 Ver: 0x");
  usart_send_u8(adxl345_read(0x00));
  usart_send_string("\r\n");
  adxl345_write(0x31, 0x0b);
  adxl345_write(0x2d, 0x08);
  adxl345_write(0x2c, 0x0e);
  adxl345_write(0x2e, 0x00);
  adxl345_write(0x38, 0x00);
  u16 buffer[3];
  adxl345_read_xyz((u8*)buffer);
  usart_send_u8_array((u8*)buffer, 6);
  usart_send_string("\r\n");
}

void adxl345_write(vu8 addr, u8 data)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  gpio_set(ADXL345_GPIO, ADXL345_NSS, LOW);
  spiInOut(ADXL345_SPI, addr & 0x3f);
  spiInOut(ADXL345_SPI, data);
  gpio_set(ADXL345_GPIO, ADXL345_NSS, HIGH);
}

u8 adxl345_read(vu8 addr)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  u8 v = 0;
  gpio_set(ADXL345_GPIO, ADXL345_NSS, LOW);
  spiInOut(ADXL345_SPI, (addr & 0x3f) | 0x80);
  v = spiInOut(ADXL345_SPI, 0);
  gpio_set(ADXL345_GPIO, ADXL345_NSS, HIGH);
  return(v);
}

void adxl345_read_xyz(u8* buffer)
{
  gpio_set(ADXL345_GPIO, ADXL345_NSS, LOW);
  spiInOut(ADXL345_SPI, (0x32 & 0x3f) | 0xc0);
  for(int i = 0; i < 6; i++)
  {
    *(buffer + i) = spiInOut(ADXL345_SPI, 0);
  }
  gpio_set(ADXL345_GPIO, ADXL345_NSS, HIGH);
}
