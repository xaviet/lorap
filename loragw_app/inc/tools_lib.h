/*
 * tools_lib.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef TOOLS_LIB_H_
#define TOOLS_LIB_H_

#include "stm32f10x.h"

vu8 crc8(vu8* chars, u8 length);
void delay_1us(vu32 n);
vu8 get_char_from_hex(vu8 value);
vu32 delta_ticket(vu32 startTicket, vu32 currentTicket);
void call_fun(void (*f)());
u8 stringCmp(vu8* s1, vu8* s2, u32 length);
vu32 bytesToU32(vu8* s);
void u32ToBytes(vu8* s, vu32 i);

#endif /* TOOLS_LIB_H_ */
