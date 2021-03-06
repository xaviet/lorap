/*
 * drv_sx1278.c
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#include "hw_config.h"
#include "drv_sx1278.h"
#include "drv_spi.h"
#include "drv_gpio.h"
#include "drv_usart.h"
#include "tools_lib.h"
#include "string.h"

void init_sx1278()
{
  // reset
  gpio_init(SX1278_RESET, SX1278_RESET_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
  sx1278_reset();

  sx1278_set_opmode(Sleep_mode);
  delay_1us(16);
  sx1278_write(sx1278_opMode,0x88);
  delay_1us(16);
  sx1278_write(sx1278_payloadLength, 0x80);
  sx1278_set_opmode(Stdby_mode);
  delay_1us(16);
  sx1278_write(sx1278_irqFlagsMask,0xb7);
  if(sx1278_read(sx1278_opMode) != 0x89)
  {
    usart_send_string("\tsx1278 Fault...\r\n");
  }
  else
  {
    usart_send_string("\tSx1278 Ver: 0x");
    usart_send_u8(sx1278_read(sx1278_version));
    usart_send_string("\r\n");
  }
}

void sx1278_reset()
{
  gpio_set(SX1278_RESET_GPIO, SX1278_RESET, LOW);
  delay_1us(10000);
  gpio_set(SX1278_RESET_GPIO, SX1278_RESET, HIGH);
  delay_1us(10000);
}

void sx1278_write_buffer(vu8 addr, vu8* buffer, vu8 size)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  vu8 i;
  gpio_set(SX1278_GPIO, SX1278_NSS, LOW);
  spiInOut(SX1278_SPI, addr | 0x80);
  for(i = 0; i < size; i++)
  {
    spiInOut(SX1278_SPI, buffer[i]);
  }
  gpio_set(SX1278_GPIO, SX1278_NSS, HIGH);
}

void sx1278_read_buffer(vu8 addr, vu8* buffer, vu8 size)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  vu8 i;
  gpio_set(SX1278_GPIO, SX1278_NSS, LOW);
  spiInOut(SX1278_SPI, addr & 0x7F);
  for(i = 0; i < size; i++)
  {
    buffer[i] = spiInOut(SX1278_SPI, 0);
  }
  gpio_set(SX1278_GPIO, SX1278_NSS, HIGH);
}

void sx1278_write(vu8 addr, vu8 data)
{
  sx1278_write_buffer(addr, &data, 1);
}

vu8 sx1278_read(vu8 addr)
{
  vu8 data;
  sx1278_read_buffer(addr, &data, 1);
  return(data);
}

void sx1278_set_opmode(vu8 data)
{
  vu8 preV = sx1278_read(sx1278_opMode);
  preV = (preV & 0xf8) | data;
  sx1278_write(sx1278_opMode, preV);
}

vu8 sx1278_rx_fifo(u8* buffer)
{
  u8 rxData[COMMONBUFFERLENGTH] = {0};
  memset(&rxData, 0, COMMONBUFFERLENGTH);
  u8 size = sx1278_read(sx1278_rxNbBytes);
  if(size)
  {
    sx1278_write(sx1278_fifoAddrPtr, sx1278_read(sx1278_fifoRxCurrentAddr));
    sx1278_read_buffer(0, (u8*)&rxData, size);
    usart_send_string((char*)&rxData);usart_send_string("\r\n");
    size = data_decode(rxData, size, buffer);
  }
  return(size);
}

void sx1278_rx_continue(struct SconfigMsg* cfgMsg, vu8* dioMapping1_save)
{
  sx1278_ch_cfg(cfgMsg);
  sx1278_set_dio0mapping(dio0RxDone);
  if(dioMapping1_save)
  {
    *dioMapping1_save = dio0RxDone;
  }
  sx1278_write(sx1278_irqFlags, 0xff); // clear all interrupt
  sx1278_set_opmode(Receiver_mode); // RxContinue
}

void sx1278_ch_cfg(struct SconfigMsg* cfgMsg)
{
  sx1278_write(sx1278_frMsb, cfgMsg->frMsb);
  sx1278_write(sx1278_frMib, cfgMsg->frMib);
  sx1278_write(sx1278_frLsb, cfgMsg->frLsb);
  sx1278_write(sx1278_paConfig, cfgMsg->power);
  sx1278_write(sx1278_modemConfig1, cfgMsg->modemConfig1);
  sx1278_write(sx1278_modemConfig2, cfgMsg->modemConfig2);
  sx1278_write(sx1278_symbTimeoutLsb, cfgMsg->SymbTimeoutLsb);
  sx1278_write(sx1278_hopPeriod, cfgMsg->hopPeriod);
}

void sx1278_set_dio0mapping(u8 data)
{
  u8 preV = sx1278_read(sx1278_dioMapping1);
  preV = (preV & (~dio0Mask)) | data;
  sx1278_write(sx1278_dioMapping1, preV);
}

void sx1278_tx(struct SconfigMsg* cfgMsg, u8* buffer, u8 size, vu8* dioMapping1_save)
{
  u8 txData[COMMONBUFFERLENGTH] = {0};
  memset(&txData, 0, COMMONBUFFERLENGTH);
  size = data_encode(buffer, size, (u8*)&txData);
  usart_send_string((char*)&txData);usart_send_string("\r\n");
  sx1278_ch_cfg(cfgMsg);
  sx1278_set_dio0mapping(dio0TxDone);
  if(dioMapping1_save)
  {
    *dioMapping1_save = dio0TxDone;
  }
  sx1278_write(sx1278_fifoAddrPtr, sx1278_read(sx1278_fifoTxBaseAddr));
  sx1278_write(sx1278_payloadLength, size);
  sx1278_write_buffer(0, (u8*)&txData, size);
  sx1278_write(sx1278_irqFlags, 0xff); // clear all interrupt
  sx1278_set_opmode(Transmitter_mode); // Tx
}
