/*
 * drv_sensor.c
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#include "drv_sensor.h"
#include "global.h"
#include "tools_lib.h"
#include "drv_roadled.h"
#include "drv_itemp.h"
#include "drv_usart.h"

#define ROADLED

void init_sensor()
{

#ifdef ROADLED
  init_roadled();
#elif

#endif

}

void sensor_rw()
{
  struct SworkDataMsg* msg = &globalV.loraWorkChannelData;
  msg->msgHead.type = UPSTREAM;

#ifdef ROADLED
  struct sRoadledData* roadLedData = (struct sRoadledData*)&msg->data;
  if(roadLedData->ledSet == ON || roadLedData->ledSet == OFF)
  {
    roadled_set(roadLedData->ledSet);
  }
  u16 i = iTemp_get(4);
  roadLedData->iTemp[0] = i / 0x100;
  roadLedData->iTemp[1] = i % 0x100;
  delay_1us(8192);
  roadLedData->sensor = roadled_sensor_get();
  msg->msgHead.length = (sizeof(struct SworkDataMsg) + sizeof(struct sRoadledData) + 1) & 0xff;
#elif

#endif

  *((u8*)msg + msg->msgHead.length -1) = crc8((u8*)msg, msg->msgHead.length -1);
  usart_debug("sensor_rw");
  usart_send_u8_array((u8*)msg, msg->msgHead.length);
}
