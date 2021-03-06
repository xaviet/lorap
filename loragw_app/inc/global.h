/*
 * global.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "hw_config.h"
#include "config.h"
#include "stm32f10x.h"

struct SflashEnvValue
{
  __IO u8 envFlag; // 1: valid
  __IO u8 upgradeFlag; // 1: valid upgrade
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

struct ScommonBuffer
{
  u8 data[COMMONBUFFERLENGTH];
  u8 length;
};

struct SextiStates
{
  vu8 sx1278Busy;
  vu8 sx1278DioMapping1;
  vu8 sx1278RxDone;
  vu8 sx1278TxDone;
  vu8 w5500Int;
};

struct SstatesMachine
{
  vu32 msgId;
  vu32 startTicket;
};

struct SmsgHead
{
  u8 alignMark; // #define ALIGNMARK 0xf0
  u8 type;
  u8 length;
  u8 moteId[4];
  u8 ver;
  u8 gwId[4];
};

struct SflashEnvValueMsg
{
  struct SmsgHead msgHead;
  struct SflashEnvValue flashEnvValue;
  u8 crc8; // the last byte is value of crc8(poly:  X8 + X2 + X1 + 1)
};

struct SgwConfigMsg
{
  struct SmsgHead msgHead;
  __IO u8 envFlag; // 1: valid
  u8 ip[4];
  u8 mask[4];
  u8 mac[6];
  u8 gwIp[4];
  u8 s1Dip[4];
  u8 s1Dport[2];
  u8 s1Sport[2];
  u8 pad[4];
  u8 crc8; // the last byte is value of crc8(poly:  X8 + X2 + X1 + 1)
};

struct SfactoryConfigMsg
{
  struct SmsgHead msgHead;
  u8 id[4];
  u8 pad[32];
  u8 crc8; // the last byte is value of crc8(poly:  X8 + X2 + X1 + 1)
  };

struct SupgradeMsg
{
  struct SmsgHead msgHead;
  vu8 ver;
  u8 pos[4];
  u8 reserve1[4];
  vu8 flag;
  u8 dataSize[4];
  u8 reserve2[4];
  u8 reserve3[1];
  u8 data[COMMONBUFFERLENGTH];
};

struct SupgradeBuffer
{
  vu8 upgradeDataZone[FLASH_SECTOR_SIZE];
  vu32 upgradeDataGotSize;
  vu32 upgradeDataTotleSize;
  vu8 upgradeRepeat;
};

struct StaskComponents
{
  u8 run;                 // 程序运行标记：0-不运行，1运行
  u32 timer;              // 计时器 unit: ms
  u32 itvTime;            // 任务运行间隔时间 unit: ms
  void (*task_init_hook)(); // 要运行的任务初始化函数
  void (*task_hook)();     // 要运行的任务函数
};

struct Sw5500SocketRxData
{
  u8 sourceIp[4];
  u8 sourcePort[2];
  u8 length[2];
  u8 data[ETHERNETBUFFLENGTH];
};

struct Sw5500SocketRxBuffer
{
  u8 socket;
  u16 length;
  struct Sw5500SocketRxData data;
};

struct Sw5500SocketCfg
{
  u8 sourcePort[2];
//  u8 destinationMac[6];
  u8 destinationIp[4];
  u8 destinationPort[2];
  u8 mode;
};

struct SconfigMsg
{
  struct SmsgHead msgHead;
  u8 frMsb;
  u8 frMib;
  u8 frLsb;
  u8 power;
  u8 modemConfig1;
  u8 modemConfig2;
  u8 SymbTimeoutLsb;
  u8 hopPeriod;
  u8 snr;
  u8 rssi;
  u8 ctl;
  u8 timeout;
  u8 crc8;
};

struct SworkDataMsg
{
  u8 alignMark; // #define ALIGNMARK 0xf0
  u8 type;
  u8 length; // total length
  u8 moteId[4];
  u8 moteIdMask[4];
  u8 data[COMMONBUFFERLENGTH];
};

struct SglobalValue
{
  vu32 msTicket;
  vu32 upSeconds;
  vu8 ledStat;
  vu32 ledBlinkDiv;
  vu32 rebootPressedTime;
  struct SupgradeBuffer upgradeBuffer;
  struct SflashEnvValue flashEnvValue;
  struct SextiStates extiStates;
  struct ScommonBuffer usart1RxBuffer;
  struct ScommonBuffer usart2RxBuffer;
  struct ScommonBuffer loraRxBuffer;
  struct Sw5500SocketRxBuffer w5500RxBuffer;
  struct StaskComponents taskComps[16];
  struct SconfigMsg loraLoginChannelConfig;
  struct SconfigMsg loraWorkChannelConfig;
  struct SworkDataMsg loraWorkChannelData;
  struct SstatesMachine heartBeatStatesMachine;
  struct SstatesMachine irqCallStatesMachine;
  struct SstatesMachine forwardStatesMachine;
  struct SstatesMachine upgradeStatesMachine;
  struct SupgradeMsg upgradeMsg;
};

extern struct SglobalValue globalV;

void init_global();

#endif /* GLOBAL_H_ */
