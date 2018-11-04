/*
 * tools_lib.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "tools_lib.h"

vu8 crc8(vu8* chars, u8 length)
{
  vu8 rt = 0x00;
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

vu8 get_char_from_hex(vu8 value)
{
  vu8 rt;
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

vu32 delta_ticket(vu32 startTicket, vu32 currentTicket)
{
  vu32 rt = 0;
  if(currentTicket >= startTicket)
  {
    rt = currentTicket - startTicket;
  }
  else
  {
    rt = 0xffffffff - startTicket + currentTicket;
  }
  return(rt);
}

void call_fun(void (*f)())
{
  if(f)
  {
    (*f)();
  }
}

u8 stringCmp(vu8* s1, vu8* s2, u32 length)
{
  vu8 rt = TRUE;
  for(u32 i = 0; i < length; i++)
  {
    if(s1[i] != s2[i])
    {
      rt = FALSE;
      break;
    }
  }
  return(rt);
}

vu32 bytesToU32(vu8* s)
{
  u32 i = *(s + 0) * 0x01000000 +
          *(s + 1) * 0x00010000 +
          *(s + 2) * 0x00000100 +
          *(s + 3) * 0x00000001;
  return(i);
}

void u32ToBytes(vu8* s, vu32 i)
{
  *(s + 0) = (i & 0xff000000) / 0x01000000;
  *(s + 1) = (i & 0x00ff0000) / 0x00010000;
  *(s + 2) = (i & 0x0000ff00) / 0x00000100;
  *(s + 3) = (i & 0xff0000ff) / 0x00000001;
}
