/*
 * task_upgrade.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "string.h"
#include "task_upgrade.h"
#include "global.h"
#include "drv_flash.h"
#include "tools_lib.h"
#include "drv_usart.h"
#include "drv_w5500.h"

void upgrade_init()
{
  globalV.upgradeStatesMachine.msgId = EupgradeWaitting;
  globalV.upgradeBuffer.upgradeDataGotSize = 0;
  globalV.upgradeBuffer.upgradeDataTotleSize = 0;
  globalV.upgradeBuffer.upgradeRepeat = UPGRADE_REPEAT;
}

void upgrade_run()
{
  switch(globalV.upgradeStatesMachine.msgId)
  {
    case EupgradeWaitting:
      globalV.upgradeStatesMachine.msgId = EupgradeWaitting;
      break;
    case EupgradeRequest:
      if(upgradeRequestCheck())
      {
        globalV.upgradeStatesMachine.msgId = EupgradeReplyTx;
      }
      else
      {
        globalV.upgradeStatesMachine.msgId = EupgradeWaitting;
      }
      break;
    case EupgradeReplyTx:
      upgradeReplyTx();
      break;
    case EupgradeRunning:
      upgradeRunning();
      break;
    case EupgradeGotData:
      upgradeGotData();
      break;
    case EupgradeGotDataSave:
      upgradeGotDataSave();
      break;
    default:
      break;
  }
}

void upgradeGotDataSave()
{
  usart_debug("upgradeGotDataSave");
  u32 dataZoneStart = bytesToU32((u8*)&globalV.upgradeMsg.pos);
  u32 dataZoneSize = bytesToU32((u8*)&globalV.upgradeMsg.dataSize);
  memcpy((u8*)&globalV.upgradeBuffer.upgradeDataZone + (dataZoneStart % FLASH_SECTOR_SIZE),
         (u8*)&globalV.upgradeMsg.data, dataZoneSize);
  globalV.upgradeBuffer.upgradeDataGotSize = dataZoneStart + dataZoneSize;
//  usart_send_u32(dataZoneStart);
//  usart_send_string("\r\n");
//  usart_send_u32(dataZoneSize);
//  usart_send_string("\r\n");
//  usart_send_u32(globalV.upgradeBuffer.upgradeDataGotSize);
//  usart_send_string("\r\n");
//  usart_send_u32(globalV.upgradeBuffer.upgradeDataTotleSize);
//  usart_send_string("\r\n");
  dataZoneSize = globalV.upgradeBuffer.upgradeDataGotSize % FLASH_SECTOR_SIZE;
  if((dataZoneSize == 0x00) ||
      (globalV.upgradeBuffer.upgradeDataGotSize >= globalV.upgradeBuffer.upgradeDataTotleSize))
  {
    if(dataZoneSize == 0x00)
    {
      dataZoneSize = FLASH_SECTOR_SIZE;
    }
    u32 offset = ((globalV.upgradeBuffer.upgradeDataGotSize % FLASH_SECTOR_SIZE) > 0)?0: 1;
    u32 sectorWriting = (globalV.upgradeBuffer.upgradeDataGotSize + ((u32)FLASH_UPGRADE_PROG_ADDRESS ^ FLASH_START_ADDR))
		        / FLASH_SECTOR_SIZE - offset;
//    usart_send_u32(offset);
//    usart_send_u32(sectorWriting);
//    usart_send_string("\r\n");
    flash_erase(sectorWriting, 1);
//    usart_send_u8_array((u8*)&globalV.upgradeBuffer.upgradeDataZone, dataZoneSize);
    flash_write(sectorWriting, (u8*)&globalV.upgradeBuffer.upgradeDataZone, dataZoneSize);
//    u8 d[FLASH_SECTOR_SIZE];
//    flash_read(sectorWriting, (u8*)&d, dataZoneSize);
//    usart_send_u8_array((u8*)&d, dataZoneSize);
  }
  if(globalV.upgradeBuffer.upgradeDataGotSize < globalV.upgradeBuffer.upgradeDataTotleSize)
  {
    globalV.upgradeStatesMachine.msgId = EupgradeReplyTx;
  }
  else
  {
    flash_read(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
    globalV.flashEnvValue.upgradeFlag = 0x01;
    globalV.flashEnvValue.upgradeDataSize = globalV.upgradeBuffer.upgradeDataTotleSize;
    globalV.flashEnvValue.crc8 = crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1);
    flash_erase(FLASH_ENV_DATA_SECTOR, 1);
    flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
    usart_debug("write upgrade_flag");
    globalV.upgradeMsg.msgHead.type = GWUPGRADEREPLY;
    globalV.upgradeMsg.flag = FLAG_COMPLETE;
    *((u8*)&globalV.upgradeMsg + globalV.upgradeMsg.msgHead.length - 1) = crc8((u8*)&globalV.upgradeMsg, globalV.upgradeMsg.msgHead.length - 1);
    w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)&globalV.upgradeMsg, globalV.upgradeMsg.msgHead.length);
    usart_debug("upgrade completed, Reboot!");
    globalV.upgradeStatesMachine.msgId = EupgradeWaitting;
//    NVIC_SystemReset();
  }
}

void upgradeGotData()
{
  usart_debug("upgradeGotData");
  if((globalV.upgradeMsg.flag != FLAG_ERROR) &&
     (bytesToU32((u8*)&globalV.upgradeMsg.dataSize) <= UPGRADE_DATA_SIZE) &&
     (bytesToU32((u8*)&globalV.upgradeMsg.pos) == globalV.upgradeBuffer.upgradeDataGotSize))
  {
    globalV.upgradeStatesMachine.msgId = EupgradeGotDataSave;
  }
  else
  {
    usart_debug("upgradeGotData error");
    globalV.upgradeStatesMachine.msgId = EupgradeRunning;
  }

}

void upgradeRunning()
{
  if(delta_ticket(globalV.upgradeStatesMachine.startTicket, globalV.msTicket) >= (UPGRADE_OVERTIME * 1024))
  {
    usart_debug("upgradeRunning timeout");
    if(globalV.upgradeBuffer.upgradeRepeat--)
    {
      globalV.upgradeStatesMachine.msgId = EupgradeReplyTx;
    }
    else
    {
      usart_debug("upgradeRepeat is 0");
      globalV.upgradeStatesMachine.msgId = EupgradeWaitting;
    }
  }
  else
  {
    globalV.upgradeStatesMachine.msgId = EupgradeRunning;
  }
}

void upgradeReplyTx()
{
  globalV.upgradeMsg.msgHead.type = GWUPGRADEREPLY;
  globalV.upgradeMsg.msgHead.length = 0x21;
  u32ToBytes((u8*)&globalV.upgradeMsg.pos, globalV.upgradeBuffer.upgradeDataGotSize);
  globalV.upgradeMsg.flag = FLAG_TRANSMITTING;
  u32 dataSize = 0;
  if(globalV.upgradeBuffer.upgradeDataTotleSize - globalV.upgradeBuffer.upgradeDataGotSize >= UPGRADE_DATA_SIZE)
  {
    dataSize = UPGRADE_DATA_SIZE;
  }
  else
  {
    dataSize = globalV.upgradeBuffer.upgradeDataTotleSize - globalV.upgradeBuffer.upgradeDataGotSize;
  }
  u32ToBytes((u8*)&globalV.upgradeMsg.dataSize, dataSize);
  *((u8*)&globalV.upgradeMsg + globalV.upgradeMsg.msgHead.length - 1) = crc8((u8*)&globalV.upgradeMsg, globalV.upgradeMsg.msgHead.length - 1);
  w5500_write_socket_buffer(LORA_NS_SOCKET, (u8*)&globalV.upgradeMsg, globalV.upgradeMsg.msgHead.length);
  globalV.upgradeStatesMachine.startTicket = globalV.msTicket;
  globalV.upgradeStatesMachine.msgId = EupgradeRunning;
}

u8 upgradeRequestCheck()
{
  u8 rt = TRUE;
  usart_debug("upgradeRequestCheck");
//  usart_send_u8(globalV.upgradeMsg.ver);
//  usart_send_u8_array(globalV.upgradeMsg.pos, 4);
  globalV.upgradeBuffer.upgradeDataTotleSize = bytesToU32((u8*)&globalV.upgradeMsg.pos);
  globalV.upgradeBuffer.upgradeRepeat = UPGRADE_REPEAT;
  globalV.upgradeBuffer.upgradeDataGotSize = 0;
  return(rt);
}
