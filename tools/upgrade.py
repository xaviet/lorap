#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
upgrade.py  

'''

import time
import select
import sys
import socket
import os
import platform
import datetime

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
# tools end

def main():
  ver = 0x5a
  upgrade_req = 0xfa
  file_name = 'loragw_app.bin'
  host = ('0.0.0.0', 1700)
  gw = ('100.1.1.200', 38564)
  msg_upgrade_req = [0xf0, 
                     upgrade_req, 
                     0x21, 
                     0x00, 0x00, 0x00, 0x00, 
                     0x00, 
                     0xff, 0xff, 0xff, 0xff,
                     ver,
                     0x00, 0x00, 0x00, 0x00, 
                     0x00, 0x00, 0x00, 0x00, 
                     0x00,
                     0x00, 0x00, 0x00, 0x00, 
                     0x00, 0x00, 0x00, 0x00, 
                     0x00]
  msg_upgrade_data = [0x00]
  msg_upgrade_crc = [0x00]
  data = list(open(file_name, 'rb').read())
  data_size = len(data)
  debug('open ', file_name, str(data_size))
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(host)
  msg = msg_upgrade_req.copy()
  msg[13:17] = int_to_bytes(data_size)
  msg += msg_upgrade_data
  msg += [crc8(msg, len(msg))]
  debug('tx', ' '.join(['%02x' % (int(el0), ) for el0 in msg]))
  s.sendto(bytes(msg), gw)
  tx_start = 0
  tx_len = 0
  while(1):
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      msg = list(d)
      if(msg[1] == 0xf9):
        debug('rx', ' '.join(['%02x' % (int(el0), ) for el0 in msg]))
        tx_start = bytes_to_int(msg[13:17])
        tx_len = bytes_to_int(msg[22:26])
        debug('request', 'start', tx_start, 'size', tx_len)
        msg = msg[0:len(msg_upgrade_req)].copy()
        msg[1] = upgrade_req
        msg += data[tx_start:tx_start + tx_len]
        msg[2] = len(msg) + 1
        # if(tx_start + tx_len >= data_size):  
          # msg[21] = 2
        msg += [crc8(msg, len(msg))]
        debug('send start: ', str(tx_start), ' length: ', str(tx_len))
        debug('tx', ' '.join(['%02x' % (int(el0), ) for el0 in msg]))
        #time.sleep(0.125)
        s.sendto(bytes(msg), gw)
        if(tx_start + tx_len >= data_size):
          debug('send complete totle length: ', str(tx_start + tx_len), 'data length: ', data_size)
          break;
  while(1):
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      msg = list(d)
      debug('rx', ' '.join(['%02x' % (int(el0), ) for el0 in msg]))
      #break
        
        

if(__name__ == '__main__'):
  main()
  
  
  
  
  
  
  
  
  
  
  
