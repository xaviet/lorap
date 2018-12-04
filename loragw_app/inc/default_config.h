/*
 * default_config.h
 *
 *  Created on: Nov 2, 2018
 *      Author: mose
 */

#ifndef DEFAULT_CONFIG_H_
#define DEFAULT_CONFIG_H_

#include "stm32f10x.h"

#define GWID0 0xff
#define GWID1 0xff
#define GWID2 0xff
#define GWID3 0xff
#define VER 0x01

// 0x6c8000:433mhz 0x6c4000:432mhz bw:62.5khz sf:8(0x87) crc:on SymbTimeout:0x03ff
// 0x760000:471mhz 0x75c000:470mhz bw:62.5khz sf:10(0xa7) crc:on SymbTimeout:0x03ff
#define FRMSB 0x75
#define FRMIB 0xc0
#define FRLSB 0x00
#define POWER 0xff
#define MODEMCONFIG1 0x68
#define MODEMCONFIG2 0xa7
#define SYMBTIMEOUTLSB 0xff
#define HOPPERIOD 0x00

#define GWIP0 100
#define GWIP1 1
#define GWIP2 1
#define GWIP3 1

#define MASK0 255
#define MASK1 255
#define MASK2 255
#define MASK3 0

#define MAC0 0x48
#define MAC1 0xdf
#define MAC2 0x1c
#define MAC3 0x01
#define MAC4 0x01
#define MAC5 0xc8

#define IP0 100
#define IP1 1
#define IP2 1
#define IP3 200

#define S0SPORT0 0xea // 60013
#define S0SPORT1 0x6d
#define S0DIP0 0
#define S0DIP1 0
#define S0DIP2 0
#define S0DIP3 0
#define S0DPORT0 0x00 // 0
#define S0DPORT1 0x00

#define S1SPORT0 0x96 // 38564
#define S1SPORT1 0xa4
#define S1DIP0 100
#define S1DIP1 1
#define S1DIP2 1
#define S1DIP3 224
#define S1DPORT0 0x06 // 1700
#define S1DPORT1 0xa4

#endif /* DEFAULT_CONFIG_H_ */
