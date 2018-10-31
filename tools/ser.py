#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  ser.py
  ref : stm32flash.py -p /dev/ttyS0 -w stm32f103c8t6_stdperiph.bin -f -t -v
'''

import time
import argparse
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
  servo = 0
  while(1):
    r = s.read(256)
    r = list(r)
    if(len(r)):
      #print('\n[%10.3f recv]' % (time.time(), ))
      for el0 in r:    
        print(chr(el0), end = '')
      #time.sleep(1)
    # print()
    # print('[%10.3f send] %d' % (time.time(), servo, ))
    # s.write(bytes([servo]))
    # servo += 1
    # if(servo > 8):
      # servo = 0

if(__name__ == '__main__'):
  main(sys.argv)
