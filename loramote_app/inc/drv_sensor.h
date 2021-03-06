/*
 * drv_sensor.h
 *
 *  Created on: Nov 6, 2018
 *      Author: mose
 */

#ifndef DRV_SENSOR_H_
#define DRV_SENSOR_H_

#include "stm32f10x.h"

#define MOTE_ROADLED_UP_MSG 0x01
#define MOTE_ROADLED_DOWN_MSG 0x02
#define MOTE_RESET_MSG 0xff

void init_sensor();
u8 sensor_rw();

#endif /* DRV_SENSOR_H_ */
