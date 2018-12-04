/*
 * drv_spi.c
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#include "hw_config.h"
#include "drv_spi.h"
#include "drv_gpio.h"

void init_spi()
{
  spi_init(SX1278_SPI, SX1278_GPIO, SX1278_MISO, SX1278_MOSI, SX1278_CLK, SX1278_NSS, SPI_BaudRatePrescaler_8, SPI_CPOL_Low, SPI_CPHA_1Edge, SPI_DataSize_8b); // SPI_BaudRatePrescaler_8: 10 MHs
//  spi_init(W5500_SPI, W5500_GPIO, W5500_MISO, W5500_MOSI, W5500_CLK, W5500_NSS, SPI_BaudRatePrescaler_2); // SPI_BaudRatePrescaler_2: 40 MHss
  spi_init(ADXL345_SPI, ADXL345_GPIO, ADXL345_MISO, ADXL345_MOSI, ADXL345_CLK, ADXL345_NSS, SPI_BaudRatePrescaler_32, SPI_CPOL_High, SPI_CPHA_2Edge, SPI_DataSize_8b);
}

void spi_init(SPI_TypeDef* spiN, GPIO_TypeDef* gpio, u16 miso, u16 mosi, u16 clk, u16 nss, u16 speed, u16 cPol, u16 cPha, u16 dataSize)
{
  // 配置SPI的MISO为浮空输入
  gpio_init(miso, gpio, GPIO_Speed_50MHz, GPIO_Mode_IN_FLOATING);
  // 配置SPI的MOSI和CLK为复用推免输出
  gpio_init(mosi | clk, gpio, GPIO_Speed_50MHz, GPIO_Mode_AF_PP);
  // 配置SPI的NSS为推免输出
  gpio_init(nss, gpio, GPIO_Speed_50MHz, GPIO_Mode_Out_PP);
 //SPI同步参数初始化定义
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = dataSize;
  SPI_InitStructure.SPI_CPOL = cPol;
  SPI_InitStructure.SPI_CPHA = cPha;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = speed;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(spiN, &SPI_InitStructure);
  SPI_Cmd(spiN, ENABLE);
}

u8 spiInOut(SPI_TypeDef* SPIx, vu8 outData)
{
  // Send SPI data
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(SPIx, outData);
  while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
  return(SPI_I2S_ReceiveData(SPIx));
}
