#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  ser.py
  ref : ./stm32flash.py -w stm32f103c8t6_stdperiph.bin -f -t
  ref : ./ser.py /dev/ttyUSB0 f000001000000100
'''

import time
import serial
import sys

def main(a):
  port = '/dev/ttyUSB0' if(len(a) < 2) else a[1]
  ids = bytes([ord(el0) for el0 in list(('' if(len(a) < 3) else a[2]) + '\n')])
  print(ids)
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
      if((t != 0) and (time.time() - t > 4) and (t != -1) and len(ids) > 8):
        s.write(ids)
        t = -1
  except EnvironmentError as err:
    print(err)
  finally:
    s.close()

if(__name__ == '__main__'):
  main(sys.argv)
