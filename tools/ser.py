#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  ser.py
  ref : ./stm32flash.py -w stm32f103c8t6_stdperiph.bin -f -t
'''

import time
import serial
import sys

def main(a):
  port = '/dev/ttyUSB0' if(len(a) == 1) else a[1]
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
    while(1):
      time.sleep(0.125)
      rd = ''
      while s.inWaiting() > 0:
        rd += chr(s.read(1)[0])
      if(len(rd) > 0):
        print(rd, end = '')
        rd = ''
  except EnvironmentError as err:
    print(err)
  finally:
    s.close()

if(__name__ == '__main__'):
  main(sys.argv)
