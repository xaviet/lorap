/*
 * task_heartbeat.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "string.h"
#include "task_heartbeat.h"
#include "config.h"
#include "global.h"
#include "hw_config.h"
#include "tools_lib.h"
#include "drv_usart.h"
#include "drv_w5500.h"
#include "drv_itemp.h"

void heartbeat_init()
{

}

void heartbeat_run()
{
  switch(globalV.heartBeatStatesMachine.msgId)
  {
    case Edefault:
      heartbeatStates_default();
      break;
    case EheartBeatTx:
      heartbeatStates_tx();
      break;
    case EheartBeatRxing:
      heartbeatStates_rxing();
      break;
    case EheartBeatRxDone:
      heartbeatStates_rxDone();
      break;
    case EheartBeatDelay:
      heartbeatStates_delay();
      break;
    default:
      break;
  }
}

void heartbeatStates_delay()
{
  if(delta_ticket(globalV.heartBeatStatesMachine.startTicket, globalV.msTicket) >= HEARTBEAT_INTERVAL)
   {
    globalV.heartBeatStatesMachine.msgId = EheartBeatTx;
   }
  else
  {
    globalV.heartBeatStatesMachine.msgId = EheartBeatDelay;
  }
}

void heartbeatStates_rxDone()
{
  usart_debug("heartBeatStates_rxDone");
  globalV.ledBlinkDiv = LEDBLINKDIVDONE;
  globalV.extiStates.w5500Int = OFF;
  globalV.heartBeatStatesMachine.startTicket = globalV.msTicket;
  globalV.heartBeatStatesMachine.msgId = EheartBeatDelay;
}

void heartbeatStates_rxing()
{
  if(delta_ticket(globalV.heartBeatStatesMachine.startTicket, globalV.msTicket) >= HEARTBEAT_RX_TIMEOUT)
   {
     usart_debug("heartBeatStates_rxing timeout");
     globalV.ledBlinkDiv = LEDBLINKDIVTIMEOUT;
     globalV.heartBeatStatesMachine.startTicket = globalV.msTicket;
     globalV.heartBeatStatesMachine.msgId = EheartBeatDelay;
   }
  else
  {
    globalV.heartBeatStatesMachine.msgId = EheartBeatRxing;
  }
}

void heartbeatStates_tx()
{
  s16 t = iTemp_get(4);
  struct SconfigMsg bh;
  memcpy((u8*)&bh, (u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg));
  bh.msgHead.moteId[0] = t / 0x100;
  bh.msgHead.moteId[1] = t % 0x100;
  bh.crc8 = crc8((u8*)&bh, sizeof(struct SconfigMsg) - 1);
  w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)&bh, bh.msgHead.length);
  globalV.heartBeatStatesMachine.startTicket = globalV.msTicket;
  globalV.heartBeatStatesMachine.msgId = EheartBeatRxing;
}

void heartbeatStates_default()
{
  globalV.heartBeatStatesMachine.msgId = EheartBeatTx;
}

