/*
 * config.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f10x.h"
#include "hw_config.h"
#include "default_config.h"

#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON 1
#define HIGH 1
#define LOW 0
#define LORABUFFERMAXLENGTH 127
#define COMMONBUFFERLENGTH 256
#define TASKSTARTUPDELAY 2
#define TASKINTERVALTIME 0
#define ALIGNMARK 0xf0
#define CRC8POLY 0x07 // X8 + X2 + X1 + 1
#define LEDBLINKDIV 1024

#define INVALID 0
#define LOGINREQ 1
#define LOGINREP 2
#define UPSTREAM 3
#define DOWNSTREAM 4
#define HEARTBEATUP 5
#define HEARTBEATDOWN 6
#define GWBUSY 127
#define GWRESET 128
#define FACTORYCNF 130
#define GWUPGRADEREPLY 249
#define GWUPGRADEREQUEST 250
#define GWCONFIGREPLY 251
#define GWCONFIGREQUERY 252
#define GWSETUPREPLY 253
#define GWCONFIGSETUP 254
#define ETHERNETBUFFLENGTH 2048

// typedef enum taskList
// {run_mark, timer(startup delay), time_interval, task_point}
#define TASKCOMPSREG \
struct StaskComponents taskComps[EtaskMax] = \
{ \
  {FALSE, HEARTBEAT_RX_TIMEOUT, TASKINTERVALTIME, heartBeat_init, heartBeat_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, irqCall_init, irqCall_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, forward_init, forward_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, reboot_init, reboot_run}, \
  {FALSE, TASKSTARTUPDELAY, TASKINTERVALTIME, upgrade_init, upgrade_run}, \
};
typedef enum EtaskList
{
  EheartBeat = 0,
  EirqCall,
  Eforward,
  Ereboot,
  Eupgrade,
  EtaskMax,
}taskList;

typedef enum EstatesMachine
{
  Edefault = 0,
  EheartBeatTx,
  EheartBeatRxing,
  EheartBeatRxDone,
  EheartBeatDelay,
  EloginChannelListen,
  Erxing,
  EloginRequestRx,
  EloginReplyRx,
  EloginReplyTxing,
  EloginReplyTxDone,
  EdownStreamRx,
  EdownStreamTxing,
  EdownStreamTxDone,
  EupStreamRx,
  EupStreamRxing,
  EupStreamRxDone,
  EupgradeWaitting,
  EupgradeRequest,
  EupgradeReplyTx,
  EupgradeRunning,
  EupgradeGotData,
  EupgradeGotDataSave,
  Eloop = 0xffffffff,
}statesMachine;

#endif /* CONFIG_H_ */