/*
 * tools_lib.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef TOOLS_LIB_H_
#define TOOLS_LIB_H_

#include "global.h"

u8 crc8(u8* chars, u8 length);
void delay_1us(vu32 n);
u8 get_char_from_hex(u8 value);

#endif /* TOOLS_LIB_H_ */
