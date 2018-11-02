#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  burn.py
  ref : burn.py g: gw/m: mote
'''

import os
import sys

def main(sys_arg):
  if(len(sys_arg) < 2):
    return(False)
  if(sys_arg[1].strip() == 'g'):
    os.system('./hex2bin.py g')
    os.system('./stm32flash.py -w gw.bin -f')
  elif(sys_arg[1].strip() == 'm'):
    os.system('./hex2bin.py g')
    os.system('./stm32flash.py -w mote.bin -f')
if(__name__ == '__main__'):
  main(sys.argv)
