/*
 * drv_itemp.h
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#ifndef DRV_ITEMP_H_
#define DRV_ITEMP_H_

#include "stm32f10x.h"

void init_iTemp();
vs16 iTemp_get(vu8 times);

#endif /* DRV_ITEMP_H_ */
