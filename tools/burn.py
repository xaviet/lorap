#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  burn.py
  ref : burn.py g: gw/m: mote
'''

import os
import sys

def burnGw():
  os.system('./hex2bin.py g')
  os.system('./stm32flash.py -w gw.bin -f')
  
def burnMote():
  os.system('./hex2bin.py m')
  os.system('./stm32flash.py -w mote.bin -f -v') 

def main(sys_arg):
  if(len(sys_arg) < 2):
    burnMote()
  elif(sys_arg[1].strip() == 'g'):
    burnGw()
  elif(sys_arg[1].strip() == 'm'):
    burnMote()
  print('burn ok, open ser')
  os.system('./ser.py')
    
if(__name__ == '__main__'):
  main(sys.argv)
