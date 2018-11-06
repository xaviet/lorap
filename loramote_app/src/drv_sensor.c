/*
 * drv_sensor.c
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#include "drv_sensor.h"
#include "global.h"
#include "tools_lib.h"

void sensor_rw()
{
  struct SworkDataMsg* msg = &globalV.loraWorkChannelData;
  msg->msgHead.type = UPSTREAM;
  *((u8*)msg + msg->msgHead.length -1) = crc8((u8*)msg, msg->msgHead.length -1);
}
