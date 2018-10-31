/*
 * config.h
 *
 *  Created on: Aug 22, 2018
 *      Author: pv
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "stm32f10x.h"
#include "string.h"

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

#define STM32F103C8T6 //64 + 20
#ifndef STM32F103C8T6
#define STM32F103RCT6 //256 + 48
#endif

#endif /* CONFIG_H_ */
