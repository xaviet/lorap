#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import time
import select
import sys
import socket
import os
import platform
import datetime
import base64
import ctypes

def time_stamp():
  return(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f'))

def debug(*args):
  print('[%s]' % (time_stamp(),), args)

def main(a):
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(('0.0.0.0', 1700))
  while(1):
    time.sleep(0)
    r, w, e = select.select([s], [], [], 0)
    if(s in r):
      d, a = s.recvfrom(2048)
      debug('rx', str(a), bytes(list(d)).decode())

if(__name__ == '__main__'):
  main(sys.argv)
