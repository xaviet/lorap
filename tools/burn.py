#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  burn.py
  ref : burn.py g: gw/m: mote
'''

import os
import sys
  
def burn_Run(p):
  os.system('./hex2bin.py {0}'.format(p[0]))
  os.system('./stm32flash.py -p {0} -w {1} -f -v'.format(p[1], p[2])) 

def main(sys_arg):
  gwp = ('g', '/dev/ttyUSB0', 'gw.bin')
  mp = ('m', '/dev/ttyUSB0', 'mote.bin')
  p = mp
  if(len(sys_arg) < 2):
    p = mp
  elif(sys_arg[1].strip() == 'g'):
    p = gwp
  burn_Run(p)
  print('burn ok, open ser')
  os.system('./ser.py {0}'.format(p[1]))
    
if(__name__ == '__main__'):
  main(sys.argv)
