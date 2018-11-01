/*
 * drv_usart.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "drv_usart.h"

void init_usart()
{
  usart_init(USART_DEBUG, USART_GPIO, USART_TX, USART_RX, USART_SPEED, USART_IRQ);
}

void usart_init(USART_TypeDef* usartN, GPIO_TypeDef* gpio, u16 txPin, u16 rxPin, u32 baudRate, u32 irqN)
{
  GPIO_InitTypeDef GPIO_InitStrue;
  USART_InitTypeDef USART_InitStrue;
  NVIC_InitTypeDef NVIC_InitStrue;
  //复位串口 -> 可以没有
//  USART_DeInit(usartN);
  // 初始化 串口对应IO口  USART1(TX-PA9/RX-PA10)
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStrue.GPIO_Pin = txPin;
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(gpio, &GPIO_InitStrue);
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStrue.GPIO_Pin = rxPin;
  GPIO_Init(gpio, &GPIO_InitStrue);
  // 初始化 串口模式状态
  USART_InitStrue.USART_BaudRate = baudRate; // 波特率
  USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
  USART_InitStrue.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
  USART_InitStrue.USART_Parity = USART_Parity_No; // 没有奇偶校验
  USART_InitStrue.USART_StopBits = USART_StopBits_1; // 一位停止位
  USART_InitStrue.USART_WordLength = USART_WordLength_8b; // 每次发送数据宽度为8位
  USART_Init(usartN, &USART_InitStrue);
  USART_Cmd(usartN, ENABLE);//使能串口
  USART_ITConfig(usartN,USART_IT_RXNE, ENABLE);//开启接收中断
  // 初始化 中断优先级
  NVIC_InitStrue.NVIC_IRQChannel = irqN;
  NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStrue.NVIC_IRQChannelSubPriority = 2;
  NVIC_Init(&NVIC_InitStrue);
}

void USART1_IRQHandler()
{
  // 串口1中断服务函数
  if(USART_GetITStatus(USART1, USART_IT_RXNE))
  {
    USART_ClearFlag(USART1,USART_IT_RXNE);
//    USART_SendData(USART1, USART_ReceiveData(USART1));
  }
}

void usart_send_byte(USART_TypeDef* USARTx, char ch)
{
  USART_SendData(USARTx, ch);
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);  //等待发送完毕
}

void usart_send_u8(u8 value)
{
  char ch;
  ch = value & 0xF0;
  ch = ch >> 4;
  usart_send_byte(USART_DEBUG, get_char_from_hex(ch));
  ch = value & 0x0F;
  usart_send_byte(USART_DEBUG, get_char_from_hex(ch));
  usart_send_byte(USART_DEBUG, ' ');
}

void usart_send_u16(u16 value)
{
  usart_send_u8((u8)((value & 0xff00) / 0x100));
  usart_send_u8((u8)((value & 0xff) / 0x1));
}

void usart_send_u32(u32 value)
{
  usart_send_u8((u8)((value & 0xff000000) / 0x1000000));
  usart_send_u8((u8)((value & 0xff0000) / 0x10000));
  usart_send_u8((u8)((value & 0xff00) / 0x100));
  usart_send_u8((u8)((value & 0xff) / 0x1));
}

void usart_send_string(char* pString)
{
  while(*pString)
  {
    usart_send_byte(USART_DEBUG, *(pString++));
  }
}

void usart_send_u8_array(u8* data, u32 length)
{
  while(length--)
  {
    usart_send_u8(*(data++));
  }
  usart_send_string("\r\n");
}
