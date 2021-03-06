/*
 * drv_spi.h
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#ifndef DRV_SPI_H_
#define DRV_SPI_H_

#include "stm32f10x.h"

void init_spi();
void spi_init(SPI_TypeDef* spiN, GPIO_TypeDef* gpio, u16 miso, u16 mosi, u16 clk, u16 nss, u16 speed, u16 cPol, u16 cPha, u16 dataSize);
u8 spiInOut(SPI_TypeDef* SPIx, vu8 outData);

#endif /* DRV_SPI_H_ */
