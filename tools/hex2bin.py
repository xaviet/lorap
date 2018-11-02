#!/usr/bin/env python3
# -*- coding: utf-8 -*-
'''
  ser.py
  ref : hex2bin.py g: gw/m: mote
'''

import sys

def create_bin(base, size, *files):
  bin_data = [0 for el0 in range(size * 1024)]
  max_addr = 0
  hex_segment_addr = 0
  for el1 in files[:-1]:
    f = open(el1, 'rt')
    for el2 in f.readlines():
      line = el2.strip()
      if(line[0] == ':'):
        if(line[7:9] == '04'):
          hex_segment_addr = int(line[9:13], 16)
        if(line[7:9] == '00' and hex_segment_addr != 0):
          for el3 in range(int(line[1:3], 16)):
            current_addr = hex_segment_addr * 0x10000 - base + int(line[3:7], 16) + el3
            max_addr = max_addr if(max_addr > current_addr) else current_addr
            bin_data[current_addr] = int(line[9 + el3 * 2: 11 + el3 * 2], 16)
        if(line[7:9] == '01'):
          break
    f.close()
    hex_segment_addr = 0
  bin_data = bytes(bin_data)[:max_addr + 1]
  print('%08x' % (len(bin_data), ), len(bin_data))
  f = open(files[-1], 'wb')
  f.write(bin_data)
  f.close()  
  
def main(sys_arg):
  if(len(sys_arg) < 2):
    return(False)
  base_addr = 0x08000000
  if(sys_arg[1].strip() == 'g'):
    create_bin(base_addr, 256, 'loragw_boot.hex', 'loragw_app.hex', 'gw.bin')
  elif(sys_arg[1].strip() == 'm'):
    create_bin(base_addr, 64, 'loramote_boot.hex', 'loramote_app.hex', 'mote.bin')


if(__name__ == '__main__'):
  main(sys.argv)
