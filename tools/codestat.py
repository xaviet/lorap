#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  codestat.py

import os

def main():
  fileType = ['c', 'h', 'cpp', 'py']
  cDir = './'
  for parent, dirNames, fileNames in os.walk(cDir):
    totalLn = 0
    for el0 in fileNames:
      try:
        if(el0.split('.')[-1] in fileType):
          fd = open(el0, 'rt')
          ln = 0
          for el1 in fd:
            if(len(el1.strip()) > 0):
              ln += 1
          print(el0, ln)
          totalLn += ln
      except:
        pass
    print('\nall lines: ', totalLn)

if(__name__ == '__main__'):
  main()
