/*
 * drv_ds18b20.h
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#ifndef DRV_DS18B20_H_
#define DRV_DS18B20_H_

#include "stm32f10x.h"

#define DS18B20_PIN GPIO_Pin_1
#define DS18B20_GPIO GPIOB
#define DS18B20_PERIPH RCC_APB2Periph_GPIOB
#define ONEWIRE_TIMEOUT 1 // ms

void init_ds18b20();
void ds18b20_set(u8 flag);
u8 ds18b20_get();
u8 ds18b20_reset();
u8 ds18b20_read_bit();
void ds18b20_write_bit(u8 d);
u8 ds18b20_read_byte();
void ds18b20_write_byte(u8 d);
void ds18b20_read_nByte(u8* data, u8 length);
void ds18b20_write_nByte(u8* data, u8 length);
void ds18b20_get_temp(u8* data);

#endif /* DRV_DS18B20_H_ */
