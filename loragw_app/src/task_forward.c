/*
 * task_forward.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "string.h"
#include "task_forward.h"
#include "drv_usart.h"
#include "global.h"
#include "drv_sx1278.h"
#include "drv_w5500.h"
#include "tools_lib.h"

void forward_init()
{

}

void forward_run()
{
  switch(globalV.forwardStatesMachine.msgId)
   {
     case Edefault:
       forward_default();
       break;
     case EloginChannelListen:
       forward_loginChannelListen();
       break;
     case Erxing:
       forward_rxing();
       break;
     case EloginRequestRx:
       forward_loginRequestRx();
       break;
     case EloginReplyRx:
       forward_loginReplyRx();
       break;
     case EloginReplyTxing:
       forward_loginReplyTxing();
       break;
     case EloginReplyTxDone:
       forward_loginReplyTxDone();
       break;
     case EdownStreamRx:
       forward_downStreamRx();
       break;
     case EdownStreamTxing:
       forward_downStreamTxing();
       break;
     case EdownStreamTxDone:
       forward_downStreamTxDone();
       break;
     case EupStreamRx:
       forward_upStreamRx();
       break;
     case EupStreamRxing:
       forward_upStreamRxing();
       break;
     case EupStreamRxDone:
       forward_upStreamRxDone();
       break;
     default:
       break;
   }
}

void forward_upStreamRxDone()
{
  usart_debug("forward_upStreamRxDone");

  memcpy((u8*)&globalV.loraWorkChannelData,
         &globalV.loraRxBuffer.data,
         ((struct SworkDataMsg*)&globalV.loraRxBuffer.data)->length);
  globalV.loraWorkChannelConfig.msgHead.type = globalV.loraWorkChannelData.type;
  globalV.loraWorkChannelConfig.snr = sx1278_read(sx1278_pktSnrValue);
  globalV.loraWorkChannelConfig.rssi = sx1278_read(sx1278_pktRssiValue);
  globalV.loraWorkChannelConfig.crc8 = crc8((u8*)&globalV.loraWorkChannelConfig, globalV.loraWorkChannelConfig.msgHead.length - 1);
  w5500_write_socket_buffer(LORA_NS_SOCKET,
                            (u8*)&globalV.loraWorkChannelConfig,
                            globalV.loraWorkChannelConfig.msgHead.length + globalV.loraWorkChannelData.length);
  globalV.extiStates.sx1278Busy = OFF;
  globalV.extiStates.sx1278RxDone = OFF;
  globalV.forwardStatesMachine.msgId = EloginChannelListen;
}

void forward_upStreamRxing()
{
  if(delta_ticket(globalV.forwardStatesMachine.startTicket, globalV.msTicket) >= (globalV.loraWorkChannelConfig.timeout * 1024))
  {
    usart_debug("forward_upStreamRxing timeout");
    usart_send_u8(globalV.extiStates.sx1278Busy);
    usart_send_u8(globalV.extiStates.sx1278RxDone);
    usart_send_u8(globalV.extiStates.sx1278TxDone);
    usart_send_u8(sx1278_read(sx1278_irqFlags));
    usart_send_u8(sx1278_read(sx1278_opMode));
    usart_send_u8(globalV.extiStates.sx1278DioMapping1);
    usart_send_u8(globalV.extiStates.sx1278DioMapping1);
    usart_send_u8(globalV.loraWorkChannelConfig.frMsb);
    usart_send_u8(globalV.loraWorkChannelConfig.frMib);
    usart_send_u8(globalV.loraWorkChannelConfig.frLsb);
    usart_send_u8(globalV.loraWorkChannelConfig.modemConfig1);
    usart_send_u8(globalV.loraWorkChannelConfig.modemConfig2);

    globalV.forwardStatesMachine.startTicket = globalV.msTicket;
    globalV.forwardStatesMachine.msgId = EloginChannelListen;
    globalV.extiStates.sx1278Busy = OFF;
  }
  else
  {
    globalV.forwardStatesMachine.msgId = EupStreamRxing;
  }
}

void forward_upStreamRx()
{
  usart_debug("forward_upStreamRx");
  sx1278_set_opmode(Stdby_mode);
  sx1278_rx_continue(&globalV.loraWorkChannelConfig, &globalV.extiStates.sx1278DioMapping1);
  globalV.forwardStatesMachine.startTicket = globalV.msTicket;
  globalV.forwardStatesMachine.msgId = EupStreamRxing;
}

void forward_downStreamTxDone()
{
  usart_debug("forward_downStreamTxDone");
  globalV.extiStates.sx1278TxDone = OFF;
  globalV.forwardStatesMachine.msgId = EupStreamRx;
}

void forward_downStreamTxing()
{
  globalV.forwardStatesMachine.msgId = EdownStreamTxing;
}

void forward_downStreamRx()
{
  memcpy((u8*)&globalV.loraWorkChannelConfig,
         &globalV.w5500RxBuffer.data.data,
         sizeof(struct SconfigMsg) + sizeof(struct SworkDataMsg));
  struct SconfigMsg* cfgP = &globalV.loraWorkChannelConfig;
  struct SworkDataMsg* dataP = &globalV.loraWorkChannelData;
  if(!globalV.extiStates.sx1278Busy)
  {
    usart_debug("forward_downStreamRx sx1278_tx");
    globalV.extiStates.sx1278Busy = ON;
    sx1278_set_opmode(Stdby_mode);
    sx1278_tx(cfgP,
              (u8*)dataP,
              dataP->length,
              &globalV.extiStates.sx1278DioMapping1);
    globalV.forwardStatesMachine.msgId = EdownStreamTxing;
  }
  else
  {
    usart_debug("forward_downStreamRx BUSY");
    cfgP->msgHead.type = GWBUSY;
    cfgP->crc8 = crc8((u8*)cfgP, cfgP->msgHead.length - 1);
    w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)cfgP, cfgP->msgHead.length);
    globalV.forwardStatesMachine.msgId = Erxing;
  }
  globalV.extiStates.w5500Int = OFF;
}


void forward_loginReplyTxDone()
{
  usart_debug("forward_loginReplyTxDone");
  globalV.extiStates.sx1278Busy = OFF;
  globalV.extiStates.sx1278TxDone = OFF;
  globalV.forwardStatesMachine.msgId = EloginChannelListen;
}

void forward_loginReplyTxing()
{
  globalV.forwardStatesMachine.msgId = EloginReplyTxing;
}

void forward_loginReplyRx()
{
  if(!globalV.extiStates.sx1278Busy)
  {
    usart_debug("forward_rxLoginReply sx1278_tx");
    globalV.extiStates.sx1278Busy = ON;
    sx1278_set_opmode(Stdby_mode);
    sx1278_tx(&globalV.loraLoginChannelConfig,
              (u8*)&globalV.w5500RxBuffer.data.data,
              sizeof(struct SconfigMsg),
              &globalV.extiStates.sx1278DioMapping1);
    globalV.forwardStatesMachine.msgId = EloginReplyTxing;
  }
  else
  {
    usart_debug("forward_rxLoginReply BUSY");
    struct SconfigMsg* msg = (struct SconfigMsg*)&globalV.w5500RxBuffer.data.data;
    msg->msgHead.type = GWBUSY;
    msg->crc8 = crc8((u8*)msg, msg->msgHead.length - 1);
    w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)msg, msg->msgHead.length);
    globalV.forwardStatesMachine.msgId = Erxing;
  }
  globalV.extiStates.w5500Int = OFF;
}

void forward_loginRequestRx()
{
  struct SconfigMsg* msg = (struct SconfigMsg*)&globalV.loraRxBuffer.data;
  memcpy(msg->msgHead.gwId, globalV.loraLoginChannelConfig.msgHead.gwId, 4);
  msg->snr = sx1278_read(sx1278_pktSnrValue);
  msg->rssi = sx1278_read(sx1278_pktRssiValue);
  msg->crc8 = crc8((u8*)msg, msg->msgHead.length - 1);
  w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)msg, msg->msgHead.length);
  globalV.extiStates.sx1278RxDone = OFF;
  globalV.forwardStatesMachine.msgId = Erxing;
}

void forward_rxing()
{
  globalV.forwardStatesMachine.msgId = Erxing;
}

void forward_loginChannelListen()
{
  usart_debug("forward_loginChannelListen");
  sx1278_set_opmode(Stdby_mode);
  sx1278_rx_continue(&globalV.loraLoginChannelConfig, &globalV.extiStates.sx1278DioMapping1);
  globalV.forwardStatesMachine.msgId = Erxing;
}

void forward_default()
{
  globalV.forwardStatesMachine.msgId = EloginChannelListen;
}
