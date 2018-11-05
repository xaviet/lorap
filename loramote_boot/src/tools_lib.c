/*
 * tools_lib.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "tools_lib.h"

u8 crc8(u8* chars, u8 length)
{
  u8 rt = 0x00;
  u8 i;
  while(length--)
  {
    rt = (u8)(((u32)rt ^ (*chars++)) & 0xff);
    for(i = 8; i > 0; --i)
    {
      if(rt & 0x80)
      {
        rt = (u8)((((u32)rt << 1) ^ CRC8POLY) & 0xff);
      }
      else
      {
        rt = (u8)(((u32)rt << 1) & 0xff);
      }
    }
  }
  return(rt);
}

void delay_1us(vu32 n)
{
  vu32 i = 8 * n;
  while(i--);
}

u8 get_char_from_hex(u8 value)
{
  u8 rt;
  if(value > 9)
  {
    rt = value - 10 + 65;
  }
  else
  {
    rt = value + 48;
  }
  return(rt);
}
