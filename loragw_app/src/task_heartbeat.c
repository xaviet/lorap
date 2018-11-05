/*
 * task_heartbeat.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "task_heartbeat.h"
#include "config.h"
#include "global.h"
#include "tools_lib.h"
#include "drv_usart.h"
#include "drv_w5500.h"

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
  globalV.extiStates.w5500Int = OFF;
  globalV.heartBeatStatesMachine.startTicket = globalV.msTicket;
  globalV.heartBeatStatesMachine.msgId = EheartBeatDelay;
}

void heartbeatStates_rxing()
{
  if(delta_ticket(globalV.heartBeatStatesMachine.startTicket, globalV.msTicket) >= HEARTBEAT_RX_TIMEOUT)
   {
    usart_debug("heartBeatStates_rxing timeout");
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
  w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)&globalV.loraLoginChannelConfig, globalV.loraLoginChannelConfig.msgHead.length);
  globalV.heartBeatStatesMachine.startTicket = globalV.msTicket;
  globalV.heartBeatStatesMachine.msgId = EheartBeatRxing;
}

void heartbeatStates_default()
{
  globalV.heartBeatStatesMachine.msgId = EheartBeatTx;
}
