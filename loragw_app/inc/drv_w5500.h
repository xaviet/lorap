/*
 * drv_w5500.h
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#ifndef DRV_W5500_H_
#define DRV_W5500_H_

#include "stm32f10x.h"
#include "global.h"

#define MR    0x0000
  #define RST   0x80
  #define WOL   0x20
  #define PB    0x10
  #define PPP   0x08
  #define FARP  0x02
#define GAR       0x0001
#define SUBR      0x0005
#define SHAR      0x0009
#define SIPR      0x000f
#define INTLEVEL  0x0013
#define IR        0x0015
  #define CONFLICT  0x80
  #define UNREACH   0x40
  #define PPPOE     0x20
  #define MP        0x10
#define IMR   0x0016
  #define IM_IR7    0x80
  #define IM_IR6    0x40
  #define IM_IR5    0x20
  #define IM_IR4    0x10
#define SIR   0x0017
  #define S7_INT    0x80
  #define S6_INT    0x40
  #define S5_INT    0x20
  #define S4_INT    0x10
  #define S3_INT    0x08
  #define S2_INT    0x04
  #define S1_INT    0x02
  #define S0_INT    0x01
#define SIMR  0x0018
  #define S7_IMR    0x80
  #define S6_IMR    0x40
  #define S5_IMR    0x20
  #define S4_IMR    0x10
  #define S3_IMR    0x08
  #define S2_IMR    0x04
  #define S1_IMR    0x02
  #define S0_IMR    0x01
#define RTR     0x0019
#define RCR     0x001b
#define PTIMER  0x001c
#define PMAGIC  0x001d
#define PHA     0x001e
#define PSID    0x0024
#define PMRU    0x0026
#define UIPR    0x0028
#define UPORT   0x002c
#define PHYCFGR 0x002ew5500WriteNBytes
  #define RST_PHY 0x80
  #define OPMODE  0x40
  #define DPX     0x04
  #define SPD     0x02
  #define LINK    0x01
#define VERSION  0x0039
/********************* Socket Register *******************/
#define Sn_MR   0x0000
  #define MULTI_MFEN    0x80
  #define BCASTB        0x40
  #define ND_MC_MMB     0x20
  #define UCASTB_MIP6B  0x10
  #define MR_CLOSE      0x00
  #define MR_TCP        0x01
  #define MR_UDP        0x02
  #define MR_MACRAW     0x04
#define Sn_CR   0x0001
  #define OPEN      0x01
  #define LISTEN    0x02
  #define CONNECT   0x04
  #define DISCON    0x08
  #define CLOSE     0x10
  #define SEND      0x20
  #define SEND_MAC  0x21
  #define SEND_KEEP 0x22
  #define RECV      0x40
#define Sn_IR   0x0002
  #define IR_SEND_OK    0x10
  #define IR_TIMEOUT    0x08
  #define IR_RECV       0x04
  #define IR_DISCON     0x02
  #define IR_CON        0x01
#define Sn_SR   0x0003
  #define SOCK_CLOSED       0x00
  #define SOCK_INIT         0x13
  #define SOCK_LISTEN       0x14
  #define SOCK_ESTABLISHED  0x17
  #define SOCK_CLOSE_WAIT   0x1c
  #define SOCK_UDP          0x22
  #define SOCK_MACRAW       0x02
  #define SOCK_SYNSEND      0x15
  #define SOCK_SYNRECV      0x16
  #define SOCK_FIN_WAI      0x18
  #define SOCK_CLOSING      0x1a
  #define SOCK_TIME_WAIT    0x1b
  #define SOCK_LAST_ACK     0x1d
#define Sn_PORT   0x0004
#define Sn_DHAR   0x0006
#define Sn_DIPR   0x000c
#define Sn_DPORTR 0x0010
#define Sn_MSSR   0x0012
#define Sn_TOS    0x0015
#define Sn_TTL    0x0016
#define Sn_RXBUF_SIZE 0x001e
#define Sn_TXBUF_SIZE 0x001f
#define Sn_TX_FSR 0x0020
#define Sn_TX_RD  0x0022
#define Sn_TX_WR  0x0024
#define Sn_RX_RSR 0x0026
#define Sn_RX_RD  0x0028
#define Sn_RX_WR  0x002a
#define Sn_IMR    0x002c
  #define IMR_SENDOK  0x10
  #define IMR_TIMEOUT 0x08
  #define IMR_RECV    0x04
  #define IMR_DISCON  0x02
  #define IMR_CON     0x01
#define Sn_FRAG     0x002d
#define Sn_KPALVTR  0x002f
/*******************************************************************/
/************************ SPI Control Byte *************************/
/*******************************************************************/
/* Operation mode bits */
#define VDM   0x00
#define FDM1  0x01
#define FDM2  0x02
#define FDM4  0x03
/* Read_Write control bit */
#define RWB_READ  0x00
#define RWB_WRITE 0x04
/* Block select bits */
#define COMMON_R  0x00
/* Socket 0 */
#define SOCKETn_REG    0x08
#define SOCKETn_TX_BUF 0x10
#define SOCKETn_RX_BUF 0x18

void init_w5500();
vu8 socket_open(vu8 socket);
void w5500_socket_load_parament(u8 socket, __IO struct Sw5500SocketCfg* sock);
void w5500_load_parament(struct SflashEnvValue* env);
void w5500_reset();
vu8 w5500_inout_1Byte(vu8 data);
void w5500_write_reg_addr(vu16 addr);
vu8 w5500_rw_1byte(vu16 addr, vu8 controlByte, vu8 data);
vu16 w5500_rw_2bytes(vu16 addr, vu8 controlByte, vu16 data);
vu32 w5500_rw_4bytes(vu16 addr, vu8 controlByte, vu32 data);
void w5500_read_nbytes(vu16 addr, vu8 controlByte, vu8* data, vu8 length);
void w5500_write_nbytes(vu16 addr, vu8 controlByte, vu8* data, vu8 length);
void w5500_write_socket_buffer(vu8 socket, u8 *data, vu16 length);
u16 w5500_read_socket_buffer(vu8 socket, struct Sw5500SocketRxData* data);

#endif /* DRV_W5500_H_ */
