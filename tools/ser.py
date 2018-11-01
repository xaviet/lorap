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
  s = serial.Serial(
          port,
          baudrate = 115200,
          parity = serial.PARITY_NONE,
          stopbits = 1,
          timeout = 0
        )
  while(1):
    r = s.read(256)
    r = list(r)
    if(len(r)):
      for el0 in r:    
        print(chr(el0), end = '')
      #s.write(bytes(b'hello\n'))

if(__name__ == '__main__'):
  main(sys.argv)
