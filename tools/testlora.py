#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
testlora.py  

'''

import time
import select
import sys
import socket
import os
import platform
import datetime
import base64

# tools begin

def crc8(chars, length):
  rt = 0x00
  for el0 in range(length):
    rt ^= chars[el0]
    rt = rt & 0xff
    for el1 in range(8, 0, -1):
      if(rt & 0x80):
        # // X8 + X2 + X1 + 1
        rt = ((rt << 1) ^ 0x07) & 0xff
      else:
        rt = (rt << 1) & 0xff
  return(rt) 

def time_stamp():
  return(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f'))

def debug(*args):
  print('[%s]' % (time_stamp(),), args)

def bytes_to_int(b, len = 4):
  rt = 0
  for el0 in range(len):
    rt = rt * 0x100 + b[el0]
  return(rt)

def int_to_bytes(i, len = 4):
  rt = [0x00] * len
  for el0 in range(len):
    rt[el0] = i % 0x100
    i //= 0x100
  rt.reverse()
  return(rt)

def base64Encode(vChars):
  return(list(base64.b64encode(bytes(vChars))))

def base64Decode(vChars):
  return(list(base64.b64decode(bytes(vChars))))
  
def string_replace(vChars):
  s = []
  for el0 in vChars:
    if(el0 in range(ord('a'), ord('z') + 1)):
      s += [ord('z') - (el0 - ord('a'))]
    elif(el0 in range(ord('A'), ord('Z') + 1)):
      s += [ord('Z') - (el0 - ord('A'))]
    elif(el0 in range(ord('0'), ord('9') + 1)):
      s += [ord('9') - (el0 - ord('0'))]
    else:
      s += [el0]
  return(s)
      
def encoder(chars):
  return(string_replace(base64Encode(chars)))

def decoder(chars):
  return(base64Decode(string_replace(chars)))

# tools end

def main():
  host = ('0.0.0.0', 1700)
  gw = ('100.1.1.200', 38564)
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(host)
  while(1):
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      msg = list(d)
      debug('rx', bytes(msg).decode())
      msg = decoder(msg)
      print(' '.join('%02x' % el0 for el0 in msg))
      
if(__name__ == '__main__'):
  main()
