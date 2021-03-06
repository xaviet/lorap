/*
 * global.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "config.h"

struct SflashEnvValue
{
  __IO u8 envFlag; // 0: default; 1: valid
  __IO u8 upgradeFlag; // 0: default; 1: valid upgrade
  u32 upgradeDataSize;
  u32 appAddrBase;
  u8 ver;
  u8 id[4];
  u8 ip[4];
  u8 mask[4];
  u8 mac[6];
  u8 gwIp[4];
  u8 s1Dip[4];
  u8 s1Dport[2];
  u8 s1Sport[2];
  u8 pad[108];
  __IO u8 crc8;
};

void init_global();

#endif /* GLOBAL_H_ */
