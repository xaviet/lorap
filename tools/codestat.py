#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
#  codestat.py

import os

def main():
  fileType = ['c', 'h', 'cpp', 'py', 'ld', 's']
  commont = []
  cDir = '../'
  totalLn = 0
  maxfile = [0, '']
  for parent, dirNames, fileNames in os.walk(cDir):
    if('app' + os.sep + 'src' not in parent and
       'app' + os.sep + 'inc' not in parent and
       'boot' + os.sep + 'src' not in parent and
       'boot' + os.sep + 'inc' not in parent and
       os.sep + 'tools' not in parent):
      continue
    #print(parent)
    for el0 in fileNames:
      try:
        if(el0.split('.')[-1] in fileType):
          fd = open(parent + os.sep + el0, 'rt')
          ln = 0
          for el1 in fd:
            if(len(el1.strip()) > 0
              and el1.strip()[0] not in commont):
              ln += 1
            #ln += 1
          print('{0}\t{1}'.format(str(ln), '[' + parent + os.sep + el0 + ']'))
          totalLn += ln
          if(maxfile[0] < ln):
            maxfile[0] = ln
            maxfile[1] = '[' + parent + os.sep + el0 + ']'
      except:
        pass
  print('\nAll lines: {0}\nFile max lines: {2} {1}'.format(str(totalLn), maxfile[1], str(maxfile[0])))

if(__name__ == '__main__'):
  main()
