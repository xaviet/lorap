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
      ALIGNMARK, HEARTBEATUP, sizeof(struct SconfigMsg), 0x00, 0x00, 0x00, 0x00, VER, \
      GWID0, GWID1, GWID2, GWID3, FRMSB, FRMIB, FRLSB, POWER, \
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
      memcpy(globalV.loraLoginChannelConfig.msgHead.gwId, globalV.flashEnvValue.id, 4);
      globalV.loraLoginChannelConfig.crc8 = crc8((u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg) - 1);
      usart_send_string("\tLoad env data OK\r\n");
    }
    else
    {
      globalV.flashEnvValue.envFlag = ON;
      globalV.flashEnvValue.id[0] = GWID0;
      globalV.flashEnvValue.id[1] = GWID1;
      globalV.flashEnvValue.id[2] = GWID2;
      globalV.flashEnvValue.id[3] = GWID3;
      globalV.flashEnvValue.ip[0] = IP0;
      globalV.flashEnvValue.ip[1] = IP1;
      globalV.flashEnvValue.ip[2] = IP2;
      globalV.flashEnvValue.ip[3] = IP3;
      globalV.flashEnvValue.mask[0] = MASK0;
      globalV.flashEnvValue.mask[1] = MASK1;
      globalV.flashEnvValue.mask[2] = MASK2;
      globalV.flashEnvValue.mask[3] = MASK3;
      globalV.flashEnvValue.mac[0] = MAC0;
      globalV.flashEnvValue.mac[1] = MAC1;
      globalV.flashEnvValue.mac[2] = MAC2;
      globalV.flashEnvValue.mac[3] = MAC3;
      globalV.flashEnvValue.mac[4] = MAC4;
      globalV.flashEnvValue.mac[5] = MAC5;
      globalV.flashEnvValue.gwIp[0] = GWIP0;
      globalV.flashEnvValue.gwIp[1] = GWIP1;
      globalV.flashEnvValue.gwIp[2] = GWIP2;
      globalV.flashEnvValue.gwIp[3] = GWIP3;
      globalV.flashEnvValue.s1Dip[0] = S1DIP0;
      globalV.flashEnvValue.s1Dip[1] = S1DIP1;
      globalV.flashEnvValue.s1Dip[2] = S1DIP2;
      globalV.flashEnvValue.s1Dip[3] = S1DIP3;
      globalV.flashEnvValue.s1Dport[0] = S1DPORT0;
      globalV.flashEnvValue.s1Dport[1] = S1DPORT1;
      globalV.flashEnvValue.s1Sport[0] = S1SPORT0;
      globalV.flashEnvValue.s1Sport[1] = S1SPORT1;
      globalV.flashEnvValue.ver = VER;
      globalV.flashEnvValue.upgradeFlag = 0x00;
      globalV.flashEnvValue.crc8 = crc8((u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue) - 1);
      flash_erase(FLASH_ENV_DATA_SECTOR, 1);
      flash_write(FLASH_ENV_DATA_SECTOR, (u8*)&globalV.flashEnvValue, sizeof(struct SflashEnvValue));
      usart_send_string("\tLoad default OK\r\n");
    }
    usart_send_string("\tGWID: ");
    usart_send_u8_array(globalV.flashEnvValue.id, 4);
    usart_send_string("\tIP: ");
    usart_send_u8_array(globalV.flashEnvValue.ip, 4);
    usart_send_string("\tserver IP: ");
    usart_send_u8_array(globalV.flashEnvValue.s1Dip, 4);
    usart_send_string("\tLogin config: ");
    usart_send_u8_array((u8*)&globalV.loraLoginChannelConfig, sizeof(struct SconfigMsg));
  }
