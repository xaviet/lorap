/*
 * tools_lib.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "tools_lib.h"
#include "config.h"

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

/* BASE 64 encode table */
static const u8 base64en[] = {
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
  'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
  'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
  'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'w', 'x', 'y', 'z', '0', '1', '2', '3',
  '4', '5', '6', '7', '8', '9', '+', '/',
};

/* ASCII order for BASE 64 decode, -1 in unused character */
static const u8 base64de[] = {
  /* '+', ',', '-', '.', '/', '0', '1', '2', */
      62,  -1,  -1,  -1,  63,  52,  53,  54,
  /* '3', '4', '5', '6', '7', '8', '9', ':', */
      55,  56,  57,  58,  59,  60,  61,  -1,
  /* ';', '<', '=', '>', '?', '@', 'A', 'B', */
      -1,  -1,  -1,  -1,  -1,  -1,   0,   1,
  /* 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', */
       2,   3,   4,   5,   6,   7,   8,   9,
  /* 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', */
      10,  11,  12,  13,  14,  15,  16,  17,
  /* 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', */
      18,  19,  20,  21,  22,  23,  24,  25,
  /* '[', '\', ']', '^', '_', '`', 'a', 'b', */
      -1,  -1,  -1,  -1,  -1,  -1,  26,  27,
  /* 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', */
      28,  29,  30,  31,  32,  33,  34,  35,
  /* 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', */
      36,  37,  38,  39,  40,  41,  42,  43,
  /* 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', */
      44,  45,  46,  47,  48,  49,  50,  51,
};

u32 base64_encode(const u8* in, u32 inlen, u8* out)
{
  u32 i, j;
  j = 0;
  for(i = 0; i < inlen; i++)
  {
    int s = i % 3;      // from 8 / gcd(6, 8)
    switch(s)
    {
      case 0:
        out[j++] = base64en[(u8)((in[i] >> 2) & 0x3F)];
        break;
      case 1:
        out[j++] = base64en[(u8)(((in[i-1] & 0x3) << 4) + ((in[i] >> 4) & 0xF))];
        break;
      case 2:
        out[j++] = base64en[(u8)(((in[i-1] & 0xF) << 2) + ((in[i] >> 6) & 0x3))];
        out[j++] = base64en[(u8)(in[i] & 0x3F)];
        break;
      default:
        break;
    }
  }
  /* move back */
  i -= 1;
  /* check the last and add padding */
  if((i % 3) == 0)
  {
    out[j++] = base64en[(in[i] & 0x3) << 4];
    out[j++] = BASE64_PAD;
    out[j++] = BASE64_PAD;
  }
  else if((i % 3) == 1)
  {
    out[j++] = base64en[(in[i] & 0xF) << 2];
    out[j++] = BASE64_PAD;
  }
  out[j] = 0x00;
  return(j);
}

u32 base64_decode(const u8* in, u32 inlen, u8* out)
{
  u32 i, j;
  u8 c;
  j = 0;
  for(i = 0; i < inlen; i++)
  {
    c = base64de[in[i] - BASE64DE_FIRST];
    int s = i % 4;      // from 8 / gcd(6, 8)
    if(in[i] == '=')
    {
    }
    else if(in[i] < BASE64DE_FIRST || in[i] > BASE64DE_LAST ||
            c == -1)
    {
      j = 0;
    }
    else
    {
      switch(s)
      {
        case 0:
          out[j] = (u8)(((u32)c << 2) & 0xFF);
          break;
        case 1:
          out[j++] += (u8)(((u32)c >> 4) & 0x3);
          /* if not last char with padding */
          if (i < (inlen - 3) || in[inlen - 2] != '=')
            out[j] = (u8)(((u32)c & 0xF) << 4);
          break;
        case 2:
          out[j++] += (u8)(((u32)c >> 2) & 0xF);
          /* if not last char with padding */
          if (i < (inlen - 2) || in[inlen - 1] != '=')
            out[j] =  (u8)(((u32)c & 0x3) << 6);
          break;
        case 3:
          out[j++] += (u32)c;
          break;
        default:
          break;
      }
    }
  }
  out[j] = 0x00;
  return(j);
}

vu8 char_replace(vu8 ch)
{
  vu8 rt = ch;
  if(ch >= 'a' && ch <= 'z')
  {
    rt = 'z' - (ch - 'a');
  }
  else if(ch >= 'A' && ch <= 'Z')
  {
    rt = 'Z' - (ch - 'A');
  }
  else if(ch >= '0' && ch <= '9')
  {
    rt = '9' - (ch - '0');
  }
  return(rt);
}

void string_replace(vu8* in, vu32 len, vu8* out)
{
  for(u32 i = 0; i < len; i++)
  {
    *(out + i) = char_replace(*(in + i));
  }
}

u32 Get_serial_num(u8* id)
{
  for(int i = 0; i < 12; i++)
  {
    *(id + i) = *(UID_BASE + i);
  }
  return((*(u32*)(id + 0) >> 1) + (*(u32*)(id + 4) >> 2) + (*(u32*)(id + 0) >> 3));
}
