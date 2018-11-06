/*
 * task_irqcall.c
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#include "task_irqcall.h"
#include "tools_lib.h"
#include "drv_gpio.h"
#include "drv_flash.h"
#include "drv_usart.h"
#include "drv_timer.h"
#include "drv_exti.h"
#include "drv_sx1278.h"
#include "task.h"

void irqcall_init()
{
  sx1278_dio0_IRQ_call = sx1278_dio0_irq;
  TIM1_IRQ_call = TIM1_irq;
  TIM2_IRQ_call = TIM2_irq;
  USART1_IRQ_call = USART1_rx;
}

void irqcall_run()
{
  switch(globalV.irqCallStatesMachine.msgId)
  {
    case Edefault:
      irqCall_default();
      break;
    default:
      break;
  }
}

void irqCall_default()
{
  statesMachineJump();
  globalV.irqCallStatesMachine.msgId = Edefault;
}

void sx1278_dio0_irq()
{
  led_set(globalV.ledStat = !globalV.ledStat);
  u8 intFlag = sx1278_read(sx1278_irqFlags);
  if(((globalV.extiStates.sx1278DioMapping1 & dio0Mask) == dio0RxDone) && ((intFlag & 0x40) == 0x40))
  {
    if(globalV.extiStates.sx1278RxDone == OFF)
    {
      globalV.loraRxBuffer.length = sx1278_rx_fifo(globalV.loraRxBuffer.data);
      globalV.extiStates.sx1278RxDone = ON;
    }
  }
  if(((globalV.extiStates.sx1278DioMapping1 & dio0Mask) == dio0TxDone) && ((intFlag & 0x08) == 0x08))
  {
    globalV.extiStates.sx1278TxDone = ON;
  }
  sx1278_write(sx1278_irqFlags, intFlag);
}

void TIM1_irq()
{
  if(globalV.msTicket == 0xffffffff)
  {
    globalV.msTicket = 0;
  }
  else
  {
    globalV.msTicket++;
  }
  task_timer();
  if((globalV.msTicket % globalV.ledBlinkDiv) == 0)
  {
    led_set(globalV.ledStat = !globalV.ledStat);
  }
}

void TIM2_irq()
{
  globalV.upSeconds++;
}

void USART1_rx()
{
  u8 rx = USART_ReceiveData(USART1);  // 串口1 接收
  if(globalV.usart1RxBuffer.length < COMMONBUFFERLENGTH)
  {
    globalV.usart1RxBuffer.data[globalV.usart1RxBuffer.length++] = rx;
  }
}

void statesMachineJump()
{
  struct SconfigMsg* msg = (struct SconfigMsg*)&globalV.loraRxBuffer.data;
  if(globalV.extiStates.sx1278RxDone == ON)
  {
    if(config_msg_format(msg, *((u8*)msg + msg->msgHead.length - 1), ON))
    {
      switch(msg->msgHead.type)
      {
        case LOGINREP:
          if(globalV.forwardStatesMachine.msgId == EloginMsgRxing)
          {
            globalV.forwardStatesMachine.msgId = EloginMsgRxDone;
          }
          else
          {
            globalV.extiStates.sx1278RxDone = OFF;
          }
          break;
        case DOWNSTREAM:
          if(globalV.forwardStatesMachine.msgId == EdownStreamMsgRxing)
          {
            globalV.forwardStatesMachine.msgId = EdownStreamMsgRxDone;
          }
          else
          {
            globalV.extiStates.sx1278RxDone = OFF;
          }
          break;
        default:
          globalV.extiStates.sx1278RxDone = OFF;
          break;
      }
    }
    else
    {
      globalV.extiStates.sx1278RxDone = OFF;
    }
  }
  if(globalV.extiStates.sx1278TxDone == ON)
  {
    switch(globalV.forwardStatesMachine.msgId)
    {
      case EloginMsgTxing:
        globalV.forwardStatesMachine.msgId = EloginMsgTxDone;
        break;
      case EupStreamMsgTxing:
        globalV.forwardStatesMachine.msgId = EupStreamMsgTxDone;
        break;
      default:
        globalV.extiStates.sx1278TxDone = OFF;
        break;
    }
  }
}

u8 config_msg_format(struct SconfigMsg* msg, u8 vCrc8, u8 idCheck)
{
  if(msg->msgHead.alignMark != ALIGNMARK)
  {
    usart_debug("config_msg_format alignMark error");
    return(FALSE);
  }
  if(vCrc8 != crc8((u8*)msg, msg->msgHead.length - 1))
  {
    usart_debug("msg_format crc8 error");
    return(FALSE);
  }
  if(idCheck && !stringCmp(globalV.loraLoginChannelConfig.msgHead.moteId, msg->msgHead.moteId, 4))
  {
    usart_send_u8_array(globalV.loraLoginChannelConfig.msgHead.moteId, 4);
    usart_send_u8_array(msg->msgHead.moteId, 4);
    usart_debug("msg_format gwId error");
    return(FALSE);
  }
  return(TRUE);
}

