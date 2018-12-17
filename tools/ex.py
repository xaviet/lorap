#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
ex.py  

'''

import sys
import ctypes

import requests

def json_rpc():
  # create persistent HTTP connection
  session = requests.Session()
  # as defined in https://github.com/ethereum/wiki/wiki/JSON-RPC#net_version
  method = 'apiinfo.version'
  params = []
  payload= {"jsonrpc":"2.0",
             "method":method,
             "params":params,
             "id":1}
  headers = {'Content-type': 'application/json-rpc'}
  response = session.post('http://192.168.13.16/zabbix/api_jsonrpc.php', json=payload, headers=headers)
  print('raw json response: {}'.format(response.json()))
  print('network id: {}'.format(response.json()))


def plt_ex():
  
  import matplotlib as mpl
  from mpl_toolkits.mplot3d import Axes3D
  import numpy as np
  import matplotlib.pyplot as plt

  mpl.rcParams['legend.fontsize'] = 10

  fig = plt.figure()
  ax = fig.gca(projection='3d')
  theta = np.linspace(-4 * np.pi, 4 * np.pi, 100)
  z = np.linspace(-2, 2, 100)
  r = z**2 + 1
  x = r * np.sin(theta)
  y = r * np.cos(theta)
  ax.plot(x, y, z, label='parametric curve')
  ax.legend()

  plt.show()

  import matplotlib.pyplot as plt
  import matplotlib.font_manager as font_manager
  import numpy as np
   
  POINTS = 100
  sin_list = [0] * POINTS
  indx = 0
   
  fig, ax = plt.subplots()
  ax.set_ylim([-2, 2])
  ax.set_xlim([0, POINTS])
  ax.set_autoscale_on(False)
  ax.set_xticks(range(0, 100, 10))
  ax.set_yticks(range(-2, 3, 1))
  ax.grid(True)
   
  line_sin, = ax.plot(range(POINTS), sin_list, label='Sin() output', color='cornflowerblue')
  ax.legend(loc='upper center', ncol=4, prop=font_manager.FontProperties(size=10))
   
   
  def sin_output(ax):
      global indx, sin_list, line_sin
      if indx == 20:
          indx = 0
      indx += 1
   
      sin_list = sin_list[1:] + [np.sin((indx / 10) * np.pi)]
      line_sin.set_ydata(sin_list)
      ax.draw_artist(line_sin)
      ax.figure.canvas.draw()
   
   
  timer = fig.canvas.new_timer(interval=100)
  timer.add_callback(sin_output, ax)
  timer.start()
  plt.show()


class xyz(ctypes.Structure):
  _pack_= 1
  _fields_ = [('x', ctypes.c_int16),
              ('y', ctypes.c_int16),
              ('z', ctypes.c_int16),]
              
def main(a):
  h = 16 / 4096
  s = xyz()
  v = bytes([int(el0, 16) for el0 in '26 00 FA FF D6 FE'.split(' ')])
  ctypes.memmove(ctypes.addressof(s), v, ctypes.sizeof(xyz))
  print(s.x * h, s.y * h, s.z * h)

if(__name__ == '__main__'):
  json_rpc()
  #main(sys.argv)
  
