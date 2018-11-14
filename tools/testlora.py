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
              ('GwId', ctypes.c_byte * 4),]

class msg_buffer(ctypes.Structure):
  _pack_=1
  _fields_ = [('head', msg_head),
              ('buffer', ctypes.c_int64 * 32),]

class msg_login(ctypes.Structure):
  _pack_=1
  _fields_ = [('head', msg_head),
              ('fh', ctypes.c_byte),
              ('fm', ctypes.c_byte),
              ('fl', ctypes.c_byte),
              ('tp', ctypes.c_byte),
              ('c1', ctypes.c_byte),
              ('c2', ctypes.c_byte),
              ('st', ctypes.c_byte),
              ('fHop', ctypes.c_byte),
              ('pad0', ctypes.c_byte * 3),
              ('timeout', ctypes.c_byte),
              ('crc8', ctypes.c_byte),]

class msg_roadled_stream(ctypes.Structure):
  _pack_=1
  _fields_ = [('login', msg_login),
              ('streamHead', msg_head),
              ('set', ctypes.c_byte),
              ('get', ctypes.c_byte),
              ('iTemp', ctypes.c_byte * 2),
              ('crc8', ctypes.c_byte),]

# class end

def main():
  moteListFile = 'motelist'
  readledStat = True
  msgBuffer = msg_buffer()
  msgHead = msg_head()
  msgLogin = msg_login()
  msgRoadledStream = None
  rxData = None
  txData = None
  rxMsg = None
  txMsg = None
  loginMote = None
  try:
    f = open(moteListFile, 'rt')
    d = f.read()
    f.close()
    loginMote = [int(el0, 16) for el0 in d.strip().split(' ')]
    debug('read moteListFile', loginMote)
  except:
    pass
  loginTime = 0
  host = ('0.0.0.0', 1700)
  gw = ('100.1.1.200', 38564)
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(host)
  while(1):
    time.sleep(0)
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      rxData = list(d)
      debug('rx', bytes(rxData).decode())
      rxMsg = decoder(rxData)
      debug(' '.join('%02x' % el0 for el0 in rxMsg))
      if(rxMsg[-1] != crc8(rxMsg, rxMsg[2] - 1)):
        continue
      ctypes.memmove(ctypes.addressof(msgHead), bytes(rxMsg), ctypes.sizeof(msgHead))
      if(msgHead.type == 5):
        ctypes.memmove(ctypes.addressof(msgBuffer), bytes(rxMsg), len(rxMsg))
        msgBuffer.head.type = 6
        txMsg = list(ctypes.string_at(ctypes.addressof(msgBuffer), msgBuffer.head.length))
        txMsg[-1] = crc8(txMsg, txMsg[2] - 1)
      elif(msgHead.type == 3):
        debug('rx upstream')
        pass
      elif(msgHead.type == 1):
        ctypes.memmove(ctypes.addressof(msgLogin), bytes(rxMsg), ctypes.sizeof(msgLogin))
        msgLogin.head.type = 2
        msgLogin.fm += 0x80
        msgLogin.timeout = 0x08
        loginMote = list(ctypes.string_at(ctypes.addressof(msgLogin), msgLogin.head.length))
        f = open(moteListFile, 'wt')
        f.write(' '.join(['%02x' % (el0, ) for el0 in loginMote]))
        f.close()
        debug('write moteListFile')
        txMsg = list(ctypes.string_at(ctypes.addressof(msgLogin), msgLogin.head.length))
        txMsg[-1] = crc8(txMsg, txMsg[2] - 1)
        loginTime = time.time()
    if(loginMote != None and delta_time(loginTime, 16) and txMsg == None):
      ml = msg_login()
      ctypes.memmove(ctypes.addressof(ml), bytes(loginMote), ctypes.sizeof(ml))
      msgRoadledStream = msg_roadled_stream(ml, ml.head)
      msgRoadledStream.login.head.type = msgRoadledStream.streamHead.type = 4
      readledStat = not readledStat
      msgRoadledStream.set = 0x01 if(readledStat) else 0x00
      msgRoadledStream.login.head.length = ctypes.sizeof(msg_login)
      msgRoadledStream.streamHead.length = ctypes.sizeof(msg_roadled_stream) - ctypes.sizeof(msg_login)
      txMsg = list(ctypes.string_at(ctypes.addressof(msgRoadledStream), ctypes.sizeof(msg_roadled_stream)))
      txMsg[ctypes.sizeof(msg_login) - 1] = crc8(txMsg, ctypes.sizeof(msg_login) - 1)
      txMsg[ctypes.sizeof(msg_roadled_stream) - 1] = crc8(txMsg[ctypes.sizeof(msg_login):], msgRoadledStream.streamHead.length - 1)
      debug('loginMote', txMsg)
      loginTime = time.time()
    if(txMsg != None):
      debug(' '.join('%02x' % el0 for el0 in txMsg))
      txData = encoder(txMsg)
      debug('tx', bytes(txData).decode())
      s.sendto(bytes(txData), gw)
      txMsg = None
      
if(__name__ == '__main__'):
  main()
