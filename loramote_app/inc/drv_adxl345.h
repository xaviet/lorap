/*
 * drv_adxl345.h
 *
 *  Created on: Nov 21, 2018
 *      Author: mose
 */

#ifndef DRV_ADXL345_H_
#define DRV_ADXL345_H_

#include "global.h"

void init_adxl345();
void adxl345_write(vu8 addr, u8 data);
u8 adxl345_read(vu8 addr);
void adxl345_read_xyz(u8* buffer);

#endif /* DRV_ADXL345_H_ */
