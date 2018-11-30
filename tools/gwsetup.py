#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
gwsetup.py
  ref: ./gwsetup.py 100.1.1.200 16.0.0.1 100.1.1.201 100.1.1.253

'''

import time
import select
import sys
import socket
import os
import platform
import datetime
import base64
import ctypes

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
  rt = []
  try:
    rt = base64Decode(string_replace(chars))
  except:
    print('decoder fault')
  return(rt)

def delta_time(beginTime, timeOut):
  return(time.time() - beginTime >= timeOut)

# tools end

# class

class msg_head(ctypes.Structure):
  _pack_=1
  _fields_ = [('alignMark', ctypes.c_byte),
              ('type', ctypes.c_byte),
              ('length', ctypes.c_byte),
              ('moteId', ctypes.c_byte * 4),
              ('ver', ctypes.c_byte),
              ('gwId', ctypes.c_byte * 4),]

class msg_config(ctypes.Structure):
  _pack_=1
  _fields_ = [('head', msg_head),
              ('flag', ctypes.c_byte),
              ('ip', ctypes.c_byte * 4),
              ('mask', ctypes.c_byte * 4),
              ('mac', ctypes.c_byte * 6),
              ('gwIp', ctypes.c_byte * 4),
              ('nsIp', ctypes.c_byte * 4),
              ('srcPort', ctypes.c_byte * 2),
              ('dstPort', ctypes.c_byte * 2),
              ('pad0', ctypes.c_byte * 4),
              ('crc8', ctypes.c_byte),]

# class end

def main(a):
  print('./gwsetup.py gwip gwid setip setnsip')
  gwIp = '100.1.1.200' if(len(a) < 2) else a[1]
  gwId = '16.0.0.1' if(len(a) < 3) else a[2]
  setIp = '100.1.1.200' if(len(a) < 4) else a[3]
  setNsIp = '100.1.1.224' if(len(a) < 5) else a[4]
  host = ('0.0.0.0', 1700)
  gw = (gwIp, 38564)
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(host)
  txMsg = msg_config()
  rxMsg = msg_config()
  msgHead = msg_head()
  txMsg.head.alignMark = 0xf0
  txMsg.head.type = 0xfc
  txMsg.head.length = 0x2c
  txMsg.head.ver = 0x01
  txMsg.head.gwId[0], txMsg.head.gwId[1], txMsg.head.gwId[2], txMsg.head.gwId[3] = [int(el0) for el0 in gwId.strip().split('.')]

  rxData = []
  txData = list(ctypes.string_at(ctypes.addressof(txMsg), ctypes.sizeof(txMsg)))
  txData[-1] = crc8(txData, len(txData) - 1)
  
  debug(' '.join('%02x' % el0 for el0 in txData))
  debug('tx', bytes(encoder(txData)).decode())
  s.sendto(bytes(encoder(txData)), gw)
  while(1):
    time.sleep(0)
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      debug('rx', bytes(list(d)).decode())
      rxData = decoder(bytes(list(d)))
      debug(' '.join('%02x' % el0 for el0 in rxData))
      if(rxData[-1] != crc8(rxData, rxData[2] - 1)):
        debug('crc8 error')
        continue
      ctypes.memmove(ctypes.addressof(msgHead), bytes(rxData), ctypes.sizeof(msgHead))
      if(msgHead.type & 0xff == 0xfb):
        debug('LoRaGW配置查询')
        ctypes.memmove(ctypes.addressof(rxMsg), bytes(rxData), ctypes.sizeof(rxMsg))
        ctypes.memmove(ctypes.addressof(txMsg), ctypes.addressof(rxMsg), ctypes.sizeof(txMsg))
        txMsg.head.type = 0xfe
        txMsg.ip[0], txMsg.ip[1], txMsg.ip[2], txMsg.ip[3] = [int(el0) for el0 in setIp.strip().split('.')]
        txMsg.mac[3], txMsg.mac[4], txMsg.mac[5] = txMsg.ip[1], txMsg.ip[2], txMsg.ip[3]
        txMsg.nsIp[0], txMsg.nsIp[1], txMsg.nsIp[2], txMsg.nsIp[3] = [int(el0) for el0 in setNsIp.strip().split('.')]
        txData = list(ctypes.string_at(ctypes.addressof(txMsg), ctypes.sizeof(txMsg)))
        txData[-1] = crc8(txData, len(txData) - 1)
        s.sendto(bytes(encoder(txData)), gw)
        break
  
if(__name__ == '__main__'):
  main(sys.argv)
