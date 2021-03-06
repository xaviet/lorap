/*
 * tools_lib.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef TOOLS_LIB_H_
#define TOOLS_LIB_H_

#include "stm32f10x.h"

#define BASE64BUFFERLENGTH 256

#define BASE64_PAD      '='
#define BASE64DE_FIRST  '+'
#define BASE64DE_LAST   'z'

#define BASE64_ENCODE_OUT_SIZE(s) (((s) + 2) / 3 * 4)
#define BASE64_DECODE_OUT_SIZE(s) (((s)) / 4 * 3)

vu8 crc8(vu8* chars, u8 length);
void delay_1us(vu32 n);
vu8 get_char_from_hex(vu8 value);
vu8 get_hex_from_char(vu8 value);
vu32 delta_ticket(vu32 startTicket, vu32 currentTicket);
void call_fun(void (*f)());
u8 stringCmp(vu8* s1, vu8* s2, u32 length);
vu32 bytesToU32(vu8* s);
void u32ToBytes(vu8* s, vu32 i);
u8 base64_encode(const u8* in, u8 inlen, u8* out);
u8 base64_decode(const u8* in, u8 inlen, u8* out);
vu8 char_replace(vu8 ch);
void string_replace(vu8* in, vu32 len, vu8* out);
vu8 data_encode(u8* in, vu8 size, u8* out);
vu8 data_decode(u8* in, vu8 size, u8* out);
u32 Get_serial_num(u8* id);

#endif /* TOOLS_LIB_H_ */
