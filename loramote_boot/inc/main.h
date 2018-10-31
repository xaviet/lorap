/*
 * main.h
 *
 *  Created on: Oct 16, 2018
 *      Author: pv
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "config.h"
#include "stm32f10x.h"
#include "drv_usart.h"
#include "drv_flash.h"

//|--------------|
//|flask_max_addr|
//|              |  env data (last sector)
//|  0x08XXXXX   |
//|--------------|
//|              |
//|              |  empty (?)
//|              |
//|--------------|
//|  0x0800fbff  |
//|              |  upgrade data(26K Bytes)
//|  0x08009400  |
//|--------------|
//|  0x080093ff  |
//|              |  app prog(26K Bytes)
//|  0x08002c00  |
//|--------------|
//|  0x08001fff  |
//|              |  bootloader(11K Bytes)
//|  0x08000000  |
//----------------
#define UPGRADE_DATA_ADDRESS ((void*)0x08009400)
#define APP_PROG_ADDRESS ((void*)0x08002C00)

#define DATA_SIZE ((u32)(26 * 1024))
#define CRC8POLY 0x07 // // X8 + X2 + X1 + 1

typedef void (*pFun)();

struct SflashEnvValue
{
  u8 envFlag; // 0: default; 1: valid
  u8 ip[4];
  u8 mask[4];
  u8 mac[6];
  u8 gwIp[4];
  u8 s1Dip[4];
  u8 s1Dport[2];
  u8 s1Sport[2];
  u8 ver;
  u8 upgrade_flag; // 0: default; 1: valid upgrade
  u8 crc8;
};

void jumper();
u8 crc8(u8* chars, u8 length);
void delay_1us(u32 n);

#endif /* MAIN_H_ */
