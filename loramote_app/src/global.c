/*
 * global.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "default_config.h"
#include "config.h"
#include "global.h"
#include "string.h"
#include "tools_lib.h"
#include "drv_flash.h"
#include "drv_usart.h"

struct SglobalValue globalV;

void init_global()
{
  memset(&globalV, 0, sizeof(struct SglobalValue));
  u8 initConfigMsg[] = { \
      ALIGNMARK, LOGINREQ, sizeof(struct SconfigMsg), MOTEID0, MOTEID1, MOTEID2, MOTEID3, VER, \
      0x00, 0x00, 0x00, 0x00, FRMSB, FRMIB, FRLSB, POWER, \
      MODEMCONFIG1, MODEMCONFIG2, SYMBTIMEOUTLSB, HOPPERIOD, 0x00, 0x00, 0x00, 0x08, \
      0x00, };
    memcpy(&(globalV.loraLoginChannelConfig), &initConfigMsg, sizeof(struct SconfigMsg));
    globalV.loraLoginChannelConfig.crc8 = crc8((u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg) - 1);
    globalV.ledBlinkDiv = LEDBLINKDIV;
    flash_read(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
    if((globalV.flashEnvValue.envFlag == ON) &&
      (globalV.flashEnvValue.crc8 == crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1)))
    {
      if(globalV.flashEnvValue.ver != VER)
      {
	globalV.flashEnvValue.ver = VER;
	globalV.flashEnvValue.crc8 = crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1);
	flash_erase(FLASH_ENV_DATA_SECTOR, 1);
	flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
      }
      globalV.loraLoginChannelConfig.msgHead.ver = globalV.flashEnvValue.ver;
      memcpy(globalV.loraLoginChannelConfig.msgHead.moteId, globalV.flashEnvValue.id, 4);
      globalV.loraLoginChannelConfig.crc8 = crc8((u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg) - 1);
      usart_send_string("\tLoad env data OK\r\n");
    }
    else
    {
      globalV.flashEnvValue.envFlag = ON;
      globalV.flashEnvValue.id[0] = MOTEID0;
      globalV.flashEnvValue.id[1] = MOTEID1;
      globalV.flashEnvValue.id[2] = MOTEID2;
      globalV.flashEnvValue.id[3] = MOTEID3;
      globalV.flashEnvValue.ver = VER;
      globalV.flashEnvValue.upgradeFlag = 0x00;
      globalV.flashEnvValue.crc8 = crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1);
      flash_erase(FLASH_ENV_DATA_SECTOR, 1);
      flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
      usart_send_string("\tLoad default OK\r\n");
    }
    usart_send_string("\tMOTEID: ");
    usart_send_u8_array(globalV.loraLoginChannelConfig.msgHead.moteId, 4);
    usart_send_string("\tLogin config: ");
    usart_send_u8_array((u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg));
  }
