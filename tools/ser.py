#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  ser.py
  ref : ./stm32flash.py -w stm32f103c8t6_stdperiph.bin -f -t
  ref : ./ser.py /dev/ttyUSB0 01000003
'''

import time
import serial
import sys

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
  
def main(a):
  port = '/dev/ttyUSB0' if(len(a) < 2) else a[1]
  idsLen = 0x0c
  newId = []
  if(len(a) > 2):
    newId = [0xf0, 0x00, idsLen] + [int(a[2][i:i+2],16) for i in range(0,len(a[2]),2)] + [0, 0, 0, 0] + [0]
    newId[idsLen - 1] = crc8(newId, idsLen - 1)
  ids = bytes(newId)
  print(['%02x'% (el0) for el0 in newId])
  try:
    s = serial.Serial(
            port,
            baudrate = 115200,
            parity = serial.PARITY_NONE,
            stopbits = 1,
            timeout = 0
          )
    s.setRTS(0)
    s.setDTR(0)
    t = 0
    while(1):
      time.sleep(0.125)
      rd = ''
      while s.inWaiting() > 0:
        rd += chr(s.read(1)[0])
      if(len(rd) > 0):
        print(rd, end = '')
        rd = ''
        if(t == 0):
          t = time.time()
      if((t != 0) and (time.time() - t > 4) and (t != -1) and len(ids) == idsLen):
        s.write(ids)
        t = -1
    s.close()
  except EnvironmentError as err:
    print(err)
  finally:
    pass

if(__name__ == '__main__'):
  main(sys.argv)
