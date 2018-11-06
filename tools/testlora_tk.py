#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''
testlora.py  

'''

import tkinter
from tkinter import ttk
from tkinter import messagebox
import threading 
import time
import select
import sys
import socket
import os
import platform
import datetime

# tools begin
def time_stamp():
  return(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f'))

def debug(*args):
  print('[%s]' % (time_stamp(),), args)
# tools end

# global begin
msg = {'cooperator': [], 'cooperator_loop': 1,
       'udpsocket': [], 'udpsocket_loop': 1,
       'gui_handle': 0,
       'thread_sleep': 0.125,
       'udp_socket': None,
       'gui_setup_value': {},
       'gui_loragw_value': {},
       'gui_loramote_value': {},
       'loragw_list': {},
       'loramote_list': {},
       'socket_rx': [],
       'socket_tx': []}

cooperator_command_set = {}
udpsocket_command_set = {}

def thread_command_register():
  global cooperator_command_set, udpsocket_command_set
  cooperator_command_set['worker'] = cooperator_command_worker
  cooperator_command_set['setup_start'] = cooperator_command_setup_start
  cooperator_command_set['close'] = cooperator_command_close
  udpsocket_command_set['rx_tx'] = udpsocket_command_rx_tx
  udpsocket_command_set['setup_start'] = udpsocket_command_setup_start
  udpsocket_command_set['close'] = udpsocket_command_close
# global end

# gui begin
def gui(msg):
  debug('gui thread start!')
# main windows and frame
  main_win = tkinter.Tk()
  main_win.title('test lora')
  main_win.protocol('WM_DELETE_WINDOW', gui_command_close)
  #main_win.geometry('1280x720+128+256')
  main_win.resizable(width = False, height = False)
  msg['gui_handle'] = main_win
# tab page
  tab_control = ttk.Notebook(main_win)  
  tab_setup = ttk.Frame(tab_control)  
  tab_control.add(tab_setup, text = '--Setup--')
  tab_loragw = ttk.Frame(tab_control) 
  tab_control.add(tab_loragw, text = '--LoRaGW--')
  tab_loramote = ttk.Frame(tab_control)  
  tab_control.add(tab_loramote, text = '--LoRaMote--')  
  tab_control.pack(expand = 1, fill = 'both') 
# tab LoRaGW manage
  frame_loragw = ttk.LabelFrame(tab_loragw, text = 'LoRaGW')
  frame_loragw.pack(expand = 1, fill = 'both') 
  frame_listbox_loragw = ttk.LabelFrame(frame_loragw, text = 'LoRaGW ID list: ')
  frame_listbox_loragw.pack(expand = 1, fill = 'y', side ='left') 
  listbox_loragw_value = tkinter.StringVar()
  listbox_loragw = tkinter.Listbox(frame_listbox_loragw, listvariable = listbox_loragw_value)
  listbox_loragw.pack(expand = 1, fill = 'y') 
  frame_loragw_property = ttk.LabelFrame(frame_loragw, text = 'LoRaGW ID property: ')
  frame_loragw_property.pack(expand = 1, fill = 'both', side ='top', anchor = 'n') 
  rows = 0
  # entry_loragw_id
  ttk.Label(frame_loragw_property, text = 'ID: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_id_value = tkinter.StringVar()
  entry_loragw_id = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_id_value)
  entry_loragw_id.grid(row = rows, column =1) 
  entry_loragw_id_value.set('')
  rows += 1
  # entry_loragw_mac 
  ttk.Label(frame_loragw_property, text = 'MAC: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_mac_value = tkinter.StringVar()
  entry_loragw_mac = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_mac_value)
  entry_loragw_mac.grid(row = rows, column =1) 
  entry_loragw_mac_value.set('')
  rows += 1
  # entry_loragw_ip 
  ttk.Label(frame_loragw_property, text = 'IP: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_ip_value = tkinter.StringVar()
  entry_loragw_ip = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_ip_value)
  entry_loragw_ip.grid(row = rows, column =1) 
  entry_loragw_ip_value.set('')
  rows += 1
  # entry_loragw_mask 
  ttk.Label(frame_loragw_property, text = 'mask: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_mask_value = tkinter.StringVar()
  entry_loragw_mask = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_mask_value)
  entry_loragw_mask.grid(row = rows, column =1) 
  entry_loragw_mask_value.set('255.255.255.0')
  rows += 1
  # entry_loragw_gwip 
  ttk.Label(frame_loragw_property, text = 'GW IP: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_gwip_value = tkinter.StringVar()
  entry_loragw_gwip = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_gwip_value)
  entry_loragw_gwip.grid(row = rows, column =1) 
  entry_loragw_gwip_value.set('')
  rows += 1
  # entry_loragw_SPort 
  ttk.Label(frame_loragw_property, text = 'S port: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_SPort_value = tkinter.StringVar()
  entry_loragw_SPort = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_SPort_value)
  entry_loragw_SPort.grid(row = rows, column =1) 
  entry_loragw_SPort_value.set('')
  rows += 1
  # entry_loragw_DPort 
  ttk.Label(frame_loragw_property, text = 'D port: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_DPort_value = tkinter.StringVar()
  entry_loragw_DPort = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_DPort_value)
  entry_loragw_DPort.grid(row = rows, column =1) 
  entry_loragw_DPort_value.set('1700')
  rows += 1
  # entry_loragw_version 
  ttk.Label(frame_loragw_property, text = 'version: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_version_value = tkinter.StringVar()
  entry_loragw_version = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_version_value)
  entry_loragw_version.grid(row = rows, column =1) 
  entry_loragw_version_value.set('')
  rows += 1
  # button_loragw_ok
  ttk.Button(frame_loragw_property, text = 'ok', command = gui_command_loragw_ok).grid(row = rows, columnspan = 2)
  rows += 1
  # entry_loragw_upgrade 
  ttk.Label(frame_loragw_property, text = 'upgrade file: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_upgrade_value = tkinter.StringVar()
  entry_loragw_upgrade = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_upgrade_value)
  entry_loragw_upgrade.grid(row = rows, column =1) 
  entry_loragw_upgrade_value.set('')
  rows += 1
  # button_loragw_upgrade_ok
  ttk.Button(frame_loragw_property, text = 'upgrade', command = gui_command_loragw_upgrade_ok).grid(row = rows, columnspan = 2)
  rows += 1
  # entry_loragw_timer 
  ttk.Label(frame_loragw_property, text = 'timer: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_timer_value = tkinter.StringVar()
  entry_loragw_timer = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_timer_value)
  entry_loragw_timer.grid(row = rows, column =1) 
  entry_loragw_timer_value.set('')
  rows += 1
  # entry_loragw_rx 
  ttk.Label(frame_loragw_property, text = 'Rx: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_rx_value = tkinter.StringVar()
  entry_loragw_rx = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_rx_value)
  entry_loragw_rx.grid(row = rows, column =1) 
  entry_loragw_rx_value.set('')
  rows += 1
  # entry_loragw_tx 
  ttk.Label(frame_loragw_property, text = 'Tx: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_tx_value = tkinter.StringVar()
  entry_loragw_tx = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_tx_value)
  entry_loragw_tx.grid(row = rows, column =1) 
  entry_loragw_tx_value.set('')
  rows += 1
  # entry_loragw_error 
  ttk.Label(frame_loragw_property, text = 'error: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loragw_error_value = tkinter.StringVar()
  entry_loragw_error = ttk.Entry(frame_loragw_property, textvariable = entry_loragw_error_value)
  entry_loragw_error.grid(row = rows, column =1) 
  entry_loragw_error_value.set('')
  rows += 1
# tab LoRaMote manage
  frame_loramote = ttk.LabelFrame(tab_loramote, text = 'LoRaMote')
  frame_loramote.pack(expand = 1, fill = 'both') 
  frame_listbox_loramote = ttk.LabelFrame(frame_loramote, text = 'LoRaMote ID list: ')
  frame_listbox_loramote.pack(expand = 1, fill = 'y', side ='left') 
  listbox_loramote_value = tkinter.StringVar()
  listbox_loramote = tkinter.Listbox(frame_listbox_loramote, listvariable = listbox_loramote_value)
  listbox_loramote.pack(expand = 1, fill = 'y') 
  frame_loramote_property = ttk.LabelFrame(frame_loramote, text = 'loramote ID property: ')
  frame_loramote_property.pack(expand = 1, fill = 'both', side ='top', anchor = 'n') 
  rows = 0
  # entry_loramote_id
  ttk.Label(frame_loramote_property, text = 'ID: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_id_value = tkinter.StringVar()
  entry_loramote_id = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_id_value)
  entry_loramote_id.grid(row = rows, column =1) 
  entry_loramote_id_value.set('')
  rows += 1
  # entry_loramote_loraconfig
  ttk.Label(frame_loramote_property, text = 'LoRa config: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_loraconfig_value = tkinter.StringVar()
  entry_loramote_loraconfig = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_loraconfig_value)
  entry_loramote_loraconfig.grid(row = rows, column =1) 
  entry_loramote_loraconfig_value.set('6c 80 00 ff 68 87 ff 00')
  rows += 1
  # entry_loramote_timeout
  ttk.Label(frame_loramote_property, text = 'timeout ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_timeout_value = tkinter.StringVar()
  entry_loramote_timeout = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_timeout_value)
  entry_loramote_timeout.grid(row = rows, column =1) 
  entry_loramote_timeout_value.set('8')
  rows += 1
  # entry_loramote_txdata 
  ttk.Label(frame_loramote_property, text = 'Tx data: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_txdata_value = tkinter.StringVar()
  entry_loramote_txdata = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_txdata_value)
  entry_loramote_txdata.grid(row = rows, column =1) 
  entry_loramote_txdata_value.set('')
  rows += 1
  # button_loramote_ok
  ttk.Button(frame_loramote_property, text = 'ok', command = gui_command_loramote_ok).grid(row = rows, columnspan = 2)
  rows += 1
  # entry_loramote_snr
  ttk.Label(frame_loramote_property, text = 'SNR: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_snr_value = tkinter.StringVar()
  entry_loramote_snr = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_snr_value)
  entry_loramote_snr.grid(row = rows, column =1) 
  entry_loramote_snr_value.set('')
  rows += 1
  # entry_loramote_rssi
  ttk.Label(frame_loramote_property, text = 'RSSI: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_rssi_value = tkinter.StringVar()
  entry_loramote_rssi = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_rssi_value)
  entry_loramote_rssi.grid(row = rows, column =1) 
  entry_loramote_rssi_value.set('')
  rows += 1
  # entry_loramote_version 
  ttk.Label(frame_loramote_property, text = 'version: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_version_value = tkinter.StringVar()
  entry_loramote_version = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_version_value)
  entry_loramote_version.grid(row = rows, column =1) 
  entry_loramote_version_value.set('')
  rows += 1
  # entry_loramote_rxdata 
  ttk.Label(frame_loramote_property, text = 'Rx data: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_rxdata_value = tkinter.StringVar()
  entry_loramote_rxdata = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_rxdata_value)
  entry_loramote_rxdata.grid(row = rows, column =1) 
  entry_loramote_rxdata_value.set('')
  rows += 1
  # entry_loramote_timer 
  ttk.Label(frame_loramote_property, text = 'timer: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_timer_value = tkinter.StringVar()
  entry_loramote_timer = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_timer_value)
  entry_loramote_timer.grid(row = rows, column =1) 
  entry_loramote_timer_value.set('')
  rows += 1
  # entry_loramote_rx 
  ttk.Label(frame_loramote_property, text = 'Rx: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_rx_value = tkinter.StringVar()
  entry_loramote_rx = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_rx_value)
  entry_loramote_rx.grid(row = rows, column =1) 
  entry_loramote_rx_value.set('')
  rows += 1
  # entry_loramote_tx 
  ttk.Label(frame_loramote_property, text = 'Tx: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_tx_value = tkinter.StringVar()
  entry_loramote_tx = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_tx_value)
  entry_loramote_tx.grid(row = rows, column =1) 
  entry_loramote_tx_value.set('')
  rows += 1
  # entry_loramote_error 
  ttk.Label(frame_loramote_property, text = 'error: ').grid(row = rows, column =0, sticky = 'w') 
  entry_loramote_error_value = tkinter.StringVar()
  entry_loramote_error = ttk.Entry(frame_loramote_property, textvariable = entry_loramote_error_value)
  entry_loramote_error.grid(row = rows, column =1) 
  entry_loramote_error_value.set('')
  rows += 1
# tab setup manage
  frame_setup = ttk.LabelFrame(tab_setup, text = 'Setup')
  frame_setup.grid() 
  rows = 0
  # entry_setup_listenport
  ttk.Label(frame_setup, text = 'listen port(UDP): ').grid(row = rows, column =0, sticky = 'w') 
  msg['gui_setup_value']['listenport_widget'] = tkinter.StringVar()
  entry_setup_listenport = ttk.Entry(frame_setup, textvariable = msg['gui_setup_value']['listenport_widget'])
  entry_setup_listenport.grid(row = rows, column =1, sticky = 'w') 
  msg['gui_setup_value']['listenport_widget'].set('1700')
  rows += 1
  # checkbutton_setup_heartbeat_reply
  checkbutton_setup_heartbeat_reply_value = tkinter.IntVar()
  checkbutton_setup_heartbeat_reply = ttk.Checkbutton(frame_setup, text = 'heartbeat reply', variable=checkbutton_setup_heartbeat_reply_value, state = 'enabled')
  checkbutton_setup_heartbeat_reply_value.set(1) 
  checkbutton_setup_heartbeat_reply.grid(row = rows, columnspan = 2, sticky = 'w') 
  rows += 1
  # checkbutton_setup_heartbeat_request
  checkbutton_setup_heartbeat_request_value = tkinter.IntVar()
  checkbutton_setup_heartbeat_request = ttk.Checkbutton(frame_setup, text = 'heartbeat request', variable=checkbutton_setup_heartbeat_request_value, state = 'enabled')
  checkbutton_setup_heartbeat_request_value.set(1) 
  checkbutton_setup_heartbeat_request.grid(row = rows, columnspan = 2, sticky = 'w') 
  rows += 1
  # entry_setup_round
  ttk.Label(frame_setup, text = 'round time: ').grid(row = rows, column =0, sticky = 'w') 
  entry_setup_round_value = tkinter.StringVar()
  entry_setup_round = ttk.Entry(frame_setup, textvariable = entry_setup_round_value)
  entry_setup_round.grid(row = rows, column =1, sticky = 'w') 
  entry_setup_round_value.set('32')
  rows += 1
  # button_setup_ok
  ttk.Button(frame_setup, text = 'Start', command = gui_command_setup_start).grid(row = rows, columnspan = 2)
  rows += 1
# display windows           
  main_win.mainloop()  
  debug('gui thread stop!')

def gui_command_loramote_ok():
  if(messagebox.askyesno('LoRaMote', 'config?')):
    msg['cooperator'] += ['loramote_ok']

def gui_command_loragw_upgrade_ok():
  if(messagebox.askyesno('upgrade', 'upgrade?')):
    msg['cooperator'] += ['loragw_upgrade_ok']

def gui_command_loragw_ok():
  if(messagebox.askyesno('LoRaGW', 'config?')):
    msg['cooperator'] += ['loragw_ok']

def gui_command_setup_start():
  msg['gui_setup_value']['listenport_value'] = msg['gui_setup_value']['listenport_widget'].get()
  msg['cooperator'] += ['setup_start']

def gui_command_close():
  global msg
  msg['cooperator'] += ['close']
  msg['gui_handle'].quit()
# gui end

# cooperator begin
def cooperator(msg):
  debug('cooperator thread start!')
  while(msg['cooperator_loop']):
    time.sleep(msg['thread_sleep'])
    if(len(msg['cooperator']) > 0):
      for cooperator_msg in msg['cooperator']:
        cooperator_command(msg, cooperator_msg)
      msg['cooperator'].clear()
  debug('cooperator thread stop!')      

def cooperator_command_worker(msg, cooperator_msg):
  if(len(msg['socket_rx']) > 0):
    for el0 in msg['socket_rx']:
      debug(' '.join(['%02X' % el1 for el1 in el0[0]]), el0[1])
    msg['socket_rx'].clear()
  msg['cooperator'] += ['worker'] 

def cooperator_command_setup_start(msg, cooperator_msg):
  debug(cooperator_msg, msg['gui_setup_value']['listenport_value'])
  msg['cooperator'] += ['worker'] 
  msg['udpsocket'] += [cooperator_msg] 

def cooperator_command_close(msg, cooperator_msg):
  msg['udpsocket'] += [cooperator_msg]
  msg['cooperator_loop'] = 0

def cooperator_command(msg, cooperator_msg):
  global cooperator_command_set
  cooperator_command_set[cooperator_msg](msg, cooperator_msg)
# cooperator end

# udpsocket begin
def udpsocket(msg):
  debug('udpsocket thread start!')
  while(msg['udpsocket_loop']):
    time.sleep(msg['thread_sleep'])
    if(len(msg['udpsocket']) > 0):
      for udpsocket_msg in msg['udpsocket']:
        udpsocket_command(msg, udpsocket_msg)
      msg['udpsocket'].clear()
  if(msg['udp_socket'] != None):
    msg['udp_socket'].close()
  debug('udpsocket thread stop!')

def udpsocket_command_rx_tx(msg, udpsocket_msg):  
  r, w, e = select.select([msg['udp_socket']], [], [], 0)
  if(msg['udp_socket'] in r):
    data, addr = msg['udp_socket'].recvfrom(2048)
    msg['socket_rx'] += [[list(data), addr]]
  if(len(msg['socket_tx']) > 0):
    for el0 in msg['socket_tx']:
      msg['udp_socket'].sendto(bytes(el0[0]), el0[1])
    msg['socket_tx'].clear()
  msg['udpsocket'] += ['rx_tx']

def udpsocket_command_setup_start(msg, udpsocket_msg):
  if(msg['udp_socket'] != None):
    msg['udp_socket'].close()
  msg['udp_socket'] = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  msg['udp_socket'].bind(('0.0.0.0', int(msg['gui_setup_value']['listenport_value'])))
  msg['udpsocket'] += ['rx_tx']

def udpsocket_command_close(msg, udpsocket_msg):
  msg['udpsocket_loop'] = 0

def udpsocket_command(msg, udpsocket_msg):
  global udpsocket_command_set
  udpsocket_command_set[udpsocket_msg](msg, udpsocket_msg)
# udpsocket end

def main():
  debug('test lora start!')
  if(str(platform.system()) != 'Windows'):   
    debug('please run on windows! ' + str(platform.system()))
    return()
  global msg
  thread_command_register()
  gui_thread = threading.Thread(target = gui, args = (msg, ))
  cooperator_thread = threading.Thread(target = cooperator, args = (msg, ))
  udpsocket_thread = threading.Thread(target = udpsocket, args = (msg, ))
  gui_thread.start()
  cooperator_thread.start()
  udpsocket_thread.start()
  gui_thread.join()
  cooperator_thread.join()
  udpsocket_thread.join()
  debug('test lora stop!')

if(__name__ == '__main__'):
  main()