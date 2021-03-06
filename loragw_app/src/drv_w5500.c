/*
 * drv_w5500.c
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#include "default_config.h"
#include "hw_config.h"
#include "drv_w5500.h"
#include "drv_spi.h"
#include "drv_gpio.h"
#include "drv_usart.h"
#include "string.h"
#include "tools_lib.h"

void init_w5500()
{
  // reset
   gpio_init(W5500_RESET, W5500_RESET_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
   w5500_reset();

  u8 chw[2] = {0x12, 0x34};
  u8 chr[2] = {0};
  w5500_write_nbytes(SIPR, COMMON_R | RWB_WRITE | VDM, chw, 2);
  w5500_read_nbytes(SIPR, COMMON_R | RWB_READ | VDM, chr, 2);
  if((chw[1] != chr[1]) || (chw[0] != chr[0]))
  {
    usart_send_string("\tw5500 Fault...\r\n");
  }
  else
  {
    usart_send_string("\tw5500 Ver: ");
    u8 ver = 0;
    w5500_read_nbytes(VERSION, COMMON_R | RWB_READ | VDM, &ver, 1);
    usart_send_u8(ver);
    usart_send_string("\r\n");
    w5500_load_parament(&globalV.flashEnvValue);
  }
}

vu8 socket_open(vu8 socket)
{
  vu8 rt = FALSE;
  if(w5500_rw_1byte(Sn_MR, (socket * 0x20 + SOCKETn_REG) | RWB_READ | FDM1, 0x00) == MR_UDP)
  {
    // socket UDP
    w5500_rw_1byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, CLOSE);
    delay_1us(1);
    w5500_rw_1byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, OPEN);
    if(w5500_rw_1byte(Sn_SR, (socket * 0x20 + SOCKETn_REG) | RWB_READ | FDM1, 0x00) != SOCK_UDP)
    {
      w5500_rw_1byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, CLOSE);
    }
    else
    {
      rt = TRUE;
      //w5500RW1Byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, RECV);
    }
  }
  else
  {
    // socket TCP
  }
  return(rt);
}

void w5500_socket_load_parament(u8 socket, __IO struct Sw5500SocketCfg* sock)
{
  w5500_rw_1byte(Sn_RXBUF_SIZE, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, 0x02);//Socket Rx memory size=2k
  w5500_rw_1byte(Sn_TXBUF_SIZE, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, 0x02);//Socket Tx mempry size=2k
  w5500_rw_2bytes(Sn_MSSR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, MAXFRAMELENGTH);//Socket MTU size=1460(05b4)
  w5500_rw_1byte(Sn_MR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, sock->mode);//Socket mode UDP
  if((sock->sourcePort[0] != 0x00) || (sock->sourcePort[1] != 0x00))
  {
    w5500_write_nbytes(Sn_PORT, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | VDM, sock->sourcePort, 2);
    w5500_write_nbytes(Sn_DIPR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | VDM, sock->destinationIp, 4);
    w5500_write_nbytes(Sn_DPORTR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | VDM, sock->destinationPort, 2);
    w5500_rw_1byte(Sn_IMR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, IMR_RECV | IMR_TIMEOUT);
    socket_open(socket);
  }
//  w5500_rw_1byte(Sn_IMR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, 0xe0 | IMR_RECV | IMR_SENDOK | IMR_TIMEOUT);
  w5500_rw_1byte(Sn_IR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, 0x1f);
}

void w5500_load_parament(struct SflashEnvValue* env)
{
  w5500_rw_1byte(MR, COMMON_R | RWB_WRITE | FDM1, RST);//软件复位W5500,置1有效,复位后自动清0
  delay_1us(10000);//延时10ms,自己定义
  // socket 1
  struct Sw5500SocketCfg sock = {0};
  sock.mode = MR_UDP;
  memcpy(sock.destinationIp, env->s1Dip, 4);
  memcpy(sock.destinationPort, env->s1Dport, 2);
  memcpy(sock.sourcePort, env->s1Sport, 2);
  w5500_socket_load_parament(1, &sock);
  // socket 0
  sock.mode = MR_UDP;
  sock.sourcePort[0] = S0SPORT0;//加载s0的端口号60013
  sock.sourcePort[1] = S0SPORT1;
  sock.destinationIp[0] = S0DIP0;//加载s0的目的IP地址
  sock.destinationIp[1] = S0DIP1;
  sock.destinationIp[2] = S0DIP2;
  sock.destinationIp[3] = S0DIP3;
  sock.destinationPort[0] = S0DPORT0;//加载s0的目的端口号
  sock.destinationPort[1] = S0DPORT1;
  w5500_socket_load_parament(0, &sock);

  //设置网关(GsockerOpenateway)的IP地址,Gateway_IP为4字节unsigned char数组,自己定义
  //使用网关可以使通信突破子网的局限，通过网关可以访问到其它子网或进入Internet
  w5500_write_nbytes(GAR, COMMON_R | RWB_WRITE | VDM, env->gwIp, 4);
  //设置子网掩码(MASK)值,SUB_MASK为4字节unsigned char数组,自己定义
  //子网掩码用于子网运算
  w5500_write_nbytes(SUBR, COMMON_R | RWB_WRITE | VDM, env->mask, 4);
  //设置物理地址,PHY_ADDR为6字节unsigned char数组,自己定义,用于唯一标识网络设备的物理地址值
  //该地址值需要到IEEE申请，按照OUI的规定，前3个字节为厂商代码，后三个字节为产品序号
  //如果自己定义物理地址，注意第一个字节必须为偶数
  w5500_write_nbytes(SHAR, COMMON_R | RWB_WRITE | VDM, env->mac, 6);
  //设置本机的IP地址,IP_ADDR为4字节unsigned char数组,自己定义
  //注意，网关IP必须与本机IP属于同一个子网，否则本机将无法找到网关
  w5500_write_nbytes(SIPR, COMMON_R | RWB_WRITE | VDM, env->ip, 4);
  //设置重试时间，默认为2000(200ms)
  //每一单位数值为100微秒,初始化时值设为2000(0x07D0),等于200毫秒
  w5500_rw_2bytes(RTR, COMMON_R | RWB_WRITE | FDM2, 0xFFFF);
  //设置重试次数，默认为8次
  //如果重发的次数超过设定值,则产生超时中断(相关的端口中断寄存器中的Sn_IR 超时位(TIMEOUT)置“1”)
  w5500_rw_1byte(RCR, COMMON_R | RWB_WRITE | FDM1, 8);
  //启动中断，参考W5500数据手册确定自己需要的中断类型
  //IMR_CONFLICT是IP地址冲突异常中断,IMR_UNREACH是UDP通信时，地址无法到达的异常中断
  //其它是Socket事件中断，根据需要添加
  w5500_rw_1byte(IMR, COMMON_R | RWB_WRITE | FDM1, 0x00);
  w5500_rw_1byte(SIMR,COMMON_R | RWB_WRITE | FDM1, S0_IMR | S1_IMR | S2_IMR);
  w5500_rw_1byte(IR, COMMON_R | RWB_WRITE | FDM1, 0xf0);
}

void w5500_reset()
{
  gpio_set(W5500_RESET_GPIO, W5500_RESET, LOW);
  delay_1us(10000);
  gpio_set(W5500_RESET_GPIO, W5500_RESET, HIGH);
  delay_1us(10000);
}

vu8 w5500_inout_1Byte(vu8 data)
{
  return(spiInOut(W5500_SPI, data));
}

void w5500_write_reg_addr(vu16 addr)
{
  w5500_inout_1Byte((addr & 0xff00) >> 8);//写数据高位
  w5500_inout_1Byte(addr & 0xff);  //写数据低位
}

vu8 w5500_rw_1byte(vu16 addr, vu8 controlByte, vu8 data)
{
  gpio_set(W5500_GPIO, W5500_NSS, LOW);
  w5500_write_reg_addr(addr);
  w5500_inout_1Byte(controlByte);
  vu8 rt = w5500_inout_1Byte(data);
  gpio_set(W5500_GPIO, W5500_NSS, HIGH);
  return(rt);
}


vu16 w5500_rw_2bytes(vu16 addr, vu8 controlByte, vu16 data)
{
  gpio_set(W5500_GPIO, W5500_NSS, LOW);
  w5500_write_reg_addr(addr);
  w5500_inout_1Byte(controlByte);
  vu16 rt = w5500_inout_1Byte((data & 0xff00) / 0x100) & 0xff;
  rt = ((rt & 0xff) * 0x100) + (w5500_inout_1Byte(data & 0xff) & 0xff);
  gpio_set(W5500_GPIO, W5500_NSS, HIGH);
  return(rt);
}

vu32 w5500_rw_4bytes(vu16 addr, vu8 controlByte, vu32 data)
{
  gpio_set(W5500_GPIO, W5500_NSS, LOW);
  w5500_write_reg_addr(addr);
  w5500_inout_1Byte(controlByte);
  vu32 rt = w5500_inout_1Byte((data & 0xff000000) / 0x1000000) & 0xff;
  rt = ((rt & 0xff) * 0x100) + (w5500_inout_1Byte((data & 0x00ff0000) / 0x10000) & 0xff);
  rt = ((rt & 0xffff) * 0x100) + (w5500_inout_1Byte((data & 0x0000ff00) / 0x100) & 0xff);
  rt = ((rt & 0xffffff) * 0x100) + (w5500_inout_1Byte(data & 0x000000ff) & 0xff);
  gpio_set(W5500_GPIO, W5500_NSS, HIGH);
  return(rt);
}

void w5500_read_nbytes(vu16 addr, vu8 controlByte, vu8* data, vu8 length)
{
  gpio_set(W5500_GPIO, W5500_NSS, LOW);
  w5500_write_reg_addr(addr);
  w5500_inout_1Byte(controlByte);
  for(int i = 0; i < length; i++)
  {
    *(data + i) = w5500_inout_1Byte(*(data + i));
  }
  gpio_set(W5500_GPIO, W5500_NSS, HIGH);
}

void w5500_write_nbytes(vu16 addr, vu8 controlByte, vu8* data, vu8 length)
{
  gpio_set(W5500_GPIO, W5500_NSS, LOW);
  w5500_write_reg_addr(addr);
  w5500_inout_1Byte(controlByte);
  for(int i = 0; i < length; i++)
  {
    w5500_inout_1Byte(*(data + i));
  }
  gpio_set(W5500_GPIO, W5500_NSS, HIGH);
}

void w5500_write_socket_buffer(vu8 socket, u8 *data, vu16 length)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  u8 txData[COMMONBUFFERLENGTH] = {0};
  memset(&txData, 0, COMMONBUFFERLENGTH);
  length = data_encode(data, length, (u8*)&txData);
  usart_send_string((char*)&txData);usart_send_string("\r\n");
  if(length > 0 && length < MAXFRAMELENGTH)
  {
    u16 offset = w5500_rw_2bytes(Sn_TX_WR, (socket * 0x20 + SOCKETn_REG) | RWB_READ | FDM2, 0x0);
    if(offset + length <= 0xffff)
    {
      w5500_write_nbytes(offset, (socket * 0x20 + SOCKETn_TX_BUF) | RWB_WRITE | VDM, (u8*)&txData, length);
      w5500_rw_2bytes(Sn_TX_WR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, offset + length);
    }
    else
    {
      u16 reserver = 0xffff - offset + 1;
      u16 overflow = offset + length - 0xffff - 1;
      w5500_write_nbytes(offset, (socket * 0x20 + SOCKETn_TX_BUF) | RWB_WRITE | VDM, (u8*)&txData, reserver);
      w5500_rw_2bytes(Sn_TX_WR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, 0x0000);
      w5500_write_nbytes(0x0000, (socket * 0x20 + SOCKETn_TX_BUF) | RWB_WRITE | VDM, (u8*)&txData + reserver, overflow);
      w5500_rw_2bytes(Sn_TX_WR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, overflow);
    }
    w5500_rw_1byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, SEND);
  }
}

u16 w5500_read_socket_buffer(vu8 socket, struct Sw5500SocketRxData* data)
{
//  led_set(globalV.ledStat = !globalV.ledStat);
  struct Sw5500SocketRxData rxData = {0};
  memset((u8*)&rxData, 0, sizeof(struct Sw5500SocketRxData));
  u16 length = w5500_rw_2bytes(Sn_RX_RSR, (socket * 0x20 + SOCKETn_REG) | RWB_READ | FDM2, 0x00);
  if(length > 0 && length < MAXFRAMELENGTH)
  {
    u16 offset = w5500_rw_2bytes(Sn_RX_RD, (socket * 0x20 + SOCKETn_REG) | RWB_READ | FDM2, 0x0) ;
    if(offset + length <= 0xffff)
    {
      w5500_read_nbytes(offset, (socket * 0x20 + SOCKETn_RX_BUF) | RWB_READ | VDM, (u8*)&rxData, length);
      w5500_rw_2bytes(Sn_RX_RD, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, offset + length);
    }
    else
    {
      u16 reserver = 0xffff - offset + 1;
      u16 overflow = offset + length - 0xffff - 1;
      w5500_read_nbytes(offset, (socket * 0x20 + SOCKETn_RX_BUF) | RWB_READ | VDM, (u8*)&rxData, reserver);
      w5500_rw_2bytes(Sn_RX_RD, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, 0x0000);
      w5500_read_nbytes(0x0000, (socket * 0x20 + SOCKETn_RX_BUF) | RWB_READ | VDM, (u8*)&rxData + reserver, overflow);
      w5500_rw_2bytes(Sn_RX_RD, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM2, overflow);
    }
    w5500_rw_1byte(Sn_CR, (socket * 0x20 + SOCKETn_REG) | RWB_WRITE | FDM1, RECV);
    usart_send_string((char*)&rxData.data);usart_send_string("\r\n");
    memcpy(data->sourceIp, (u8*)&rxData.sourceIp, 4);
    memcpy(data->sourcePort, (u8*)&rxData.sourcePort, 2);
    data->length[0] = 0;
    data->length[1] = data_decode((u8*)&rxData.data, rxData.length[1], (u8*)&data->data);
  }
  else
  {
    length = 0;
  }
  return(length);
}
