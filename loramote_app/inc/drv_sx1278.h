/*
 * drv_sx1278.h
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#ifndef DRV_SX1278_H_
#define DRV_SX1278_H_

#include "global.h"

#define sx1278_fifo                             0x00
#define sx1278_opMode                           0x01
#define sx1278_frMsb                            0x06
#define sx1278_frMib                            0x07
#define sx1278_frLsb                            0x08
#define sx1278_paConfig                         0x09
#define sx1278_paRamp                           0x0a
#define sx1278_ocp                              0x0b
#define sx1278_lna                              0x0c
#define sx1278_fifoAddrPtr                      0x0d
#define sx1278_fifoTxBaseAddr                   0x0e
#define sx1278_fifoRxBaseAddr                   0x0f
#define sx1278_fifoRxCurrentAddr                0x10
#define sx1278_irqFlagsMask                     0x11
#define sx1278_irqFlags                         0x12
#define sx1278_rxNbBytes                        0x13
#define sx1278_rxHeaderCntValueMsb              0x14
#define sx1278_rxHeaderCntValueLsb              0x15
#define sx1278_rxPackerCntValueMsb              0x16
#define sx1278_rxPackerCntValueLsb              0x17
#define sx1278_modemStat                        0x18
#define sx1278_pktSnrValue                      0x19
#define sx1278_pktRssiValue                     0x1a
#define sx1278_rssiValue                        0x1b
#define sx1278_hopChannel                       0x1c
#define sx1278_modemConfig1                     0x1d
#define sx1278_modemConfig2                     0x1e
#define sx1278_symbTimeoutLsb                   0x1f
#define sx1278_preambleMsb                      0x20
#define sx1278_preambleLsb                      0x21
#define sx1278_payloadLength                    0x22
#define sx1278_maxPayloadLength                 0x23
#define sx1278_hopPeriod                        0x24
#define sx1278_fifoRxByteAddr                   0x25
#define sx1278_modemConfig3                     0x26
#define sx1278_ppmCorrection                    0x27
#define sx1278_feiMsb                           0x28
#define sx1278_feiMid                           0x29
#define sx1278_feiLsb                           0x2a
#define sx1278_rssiWideBand                     0x2c
#define sx1278_detectOptimize                   0x31
#define sx1278_invertIq                         0x33
#define sx1278_detectionThreshold               0x37
#define sx1278_syncWord                         0x39
#define sx1278_dioMapping1                      0x40
#define sx1278_dioMapping2                      0x41
#define sx1278_version                          0x42
#define sx1278_tcxo                             0x4b
#define sx1278_paDac                            0x4d
#define sx1278_formerTemp                       0x5b
#define sx1278_agcRef                           0x61
#define sx1278_agcThresh1                       0x62
#define sx1278_agcThresh2                       0x63
#define sx1278_agcTHresh3                       0x64
#define sx1278_pll                              0x70

typedef enum Esx1278OpMode
{
  Sleep_mode                      = (u8)0x00,
  Stdby_mode                      = (u8)0x01, // Default
  TX_mode                         = (u8)0x02,
  Transmitter_mode                = (u8)0x03,
  RF_mode                         = (u8)0x04,
  Receiver_mode                   = (u8)0x05,
  Receive_single                  = (u8)0x06,
  CAD_mode                        = (u8)0x07,
} sx1278OpMode;

typedef enum Esx1278Dio0Mapping
{
  dio0RxDone                      = (u8)0x00,
  dio0TxDone                      = (u8)0x40,
  dio0Mask                        = (u8)0xc0,
} sx1278Dio0;

void init_sx1278();
void sx1278_reset();
void sx1278_write_buffer(vu8 addr, vu8* buffer, vu8 size);
void sx1278_read_buffer(vu8 addr, vu8* buffer, vu8 size);
void sx1278_write(vu8 addr, vu8 data);
vu8 sx1278_read(vu8 addr);
void sx1278_set_opmode(vu8 data);
vu8 sx1278_rx_fifo(u8* buffer);
void sx1278_rx_continue(struct SconfigMsg* cfgMsg, vu8* dioMapping1_save);
void sx1278_ch_cfg(struct SconfigMsg* cfgMsg);
void sx1278_set_dio0mapping(u8 data);
void sx1278_tx(struct SconfigMsg* cfgMsg, u8* buffer, u8 size, vu8* dioMapping1_save);

#endif /* DRV_SX1278_H_ */
