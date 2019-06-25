/*
 * task_irqcall.c
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#include "string.h"
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
  //  usart_debug("sx1278_dio0_irq");
  led_set(globalV.ledStat = !globalV.ledStat);
  u8 intFlag = sx1278_read(sx1278_irqFlags);
  if(((globalV.extiStates.sx1278DioMapping1 & dio0Mask) == dio0RxDone) && ((intFlag & 0x40) == 0x40))
  {
    u8 buffer[COMMONBUFFERLENGTH] = {0};
    u8 len = sx1278_rx_fifo(buffer);
    if(globalV.extiStates.sx1278RxDone == OFF)
    {
      globalV.loraRxBuffer.length = len;
      memcpy(globalV.loraRxBuffer.data, buffer, len);
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
    USART_SendData(USART1, rx);
  }
}

void statesMachineJump()
{
  struct SmsgHead* msg = (struct SmsgHead*)&globalV.loraRxBuffer.data;
  if(globalV.extiStates.sx1278RxDone == ON)
  {
    usart_debug("sx1278RxDone");
    usart_send_u8_array((u8*)msg, msg->length);
    if(config_msg_format(msg, *((u8*)msg + msg->length - 1), ON))
    {
      switch(msg->type)
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
        case GWRESET:
          usart_debug("LoRaMote reset");
	  sx1278_set_opmode(Stdby_mode);
	  sx1278_tx(&globalV.loraWorkChannelConfig,
		    (u8*)&globalV.loraWorkChannelData,
		    globalV.loraWorkChannelConfig.msgHead.length,
		    &globalV.extiStates.sx1278DioMapping1);
	  NVIC_SystemReset();
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
  u8 idSetupLen = 0x0c;
  if(globalV.usart1RxBuffer.length >= idSetupLen)
  {
//    for(int i = 0; i < globalV.usart1RxBuffer.length; i += 2)
//    {
//      globalV.usart1RxBuffer.data[i/2] = get_hex_from_char(globalV.usart1RxBuffer.data[i]) * 0x10 +
//							   get_hex_from_char(globalV.usart1RxBuffer.data[i + 1]);
//    }
    usart_send_u8_array(globalV.usart1RxBuffer.data, globalV.usart1RxBuffer.length);
    for(int i = 0; i <= globalV.usart1RxBuffer.length - idSetupLen; i = i + 0x80)
    {
      struct SmsgHead* msgSetup = (struct SmsgHead*)(&globalV.usart1RxBuffer.data + i);
      if((config_msg_format(msgSetup, *(((u8*)msgSetup) + idSetupLen - 1), OFF)) && (*((u8*)msgSetup + 1) == FACTORYCNF))
      {
	memcpy(globalV.loraLoginChannelConfig.msgHead.gwId, globalV.usart1RxBuffer.data + 3, 4);
	memcpy(globalV.flashEnvValue.id, globalV.usart1RxBuffer.data + 3, 4);
	globalV.usart1RxBuffer.length = 0;
	globalV.flashEnvValue.crc8 = crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1);
	flash_erase(FLASH_ENV_DATA_SECTOR, 1);
	flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
	usart_debug("MoteID updta");
	usart_send_u8_array(globalV.flashEnvValue.id, 4);
	delay_1us(100000);
	NVIC_SystemReset();
      }
    }
    globalV.usart1RxBuffer.length = 0;
  }
}

u8 config_msg_format(struct SmsgHead* msg, u8 vCrc8, u8 idCheck)
{
  if(msg->alignMark != ALIGNMARK)
  {
    usart_debug("config_msg_format alignMark error");
    return(FALSE);
  }
  u8 tCrc8 = crc8((u8*)msg, msg->length - 1);
  if(vCrc8 != tCrc8)
  {
    usart_send_u8_array(&tCrc8, 1);
    usart_send_u8_array(&vCrc8, 1);
    usart_debug("msg_format crc8 error");
    return(FALSE);
  }
  if(idCheck && !stringCmp(globalV.loraLoginChannelConfig.msgHead.moteId, msg->moteId, 4))
  {
    usart_send_u8_array(globalV.loraLoginChannelConfig.msgHead.moteId, 4);
    usart_send_u8_array(msg->moteId, 4);
    usart_debug("msg_format ID error");
    return(FALSE);
  }
  return(TRUE);
}

