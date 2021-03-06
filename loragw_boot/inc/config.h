/*
 * config.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f10x.h"
#include "hw_config.h"

#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON 1
#define HIGH 1
#define LOW 0
#define LORABUFFERMAXLENGTH 127
#define COMMONBUFFERLENGTH 256
#define TASKSTARTUPDELAY 2
#define TASKINTERVALTIME 0
#define ALIGNMARK 0xf0
#define CRC8POLY 0x07 // X8 + X2 + X1 + 1

#endif /* CONFIG_H_ */
