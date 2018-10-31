/*
 * drv_usart.c
 *
 *  Created on: Aug 22, 2018
 *      Author: pv
 */

#include "drv_usart.h"

void usart_init()
{
  usart2_init(115200);
  usart1_init(115200);
}

void usart1_init(u32 baudRate)
{
  GPIO_InitTypeDef GPIO_InitStrue;
  USART_InitTypeDef USART_InitStrue;
  NVIC_InitTypeDef NVIC_InitStrue;
  // 外设使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  USART_DeInit(USART1);  //复位串口1 -> 可以没有
  // 初始化 串口对应IO口  TX-PA9  RX-PA10
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStrue);
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStrue);
  // 初始化 串口模式状态
  USART_InitStrue.USART_BaudRate = baudRate; // 波特率
  USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
  USART_InitStrue.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
  USART_InitStrue.USART_Parity = USART_Parity_No; // 没有奇偶校验
  USART_InitStrue.USART_StopBits = USART_StopBits_1; // 一位停止位
  USART_InitStrue.USART_WordLength = USART_WordLength_8b; // 每次发送数据宽度为8位
  USART_Init(USART1, &USART_InitStrue);
  USART_Cmd(USART1, ENABLE);//使能串口
  USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);//开启接收中断
  // 初始化 中断优先级
  NVIC_InitStrue.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStrue);
}

void USART1_IRQHandler(void) // 串口1中断服务函数
{
  if(USART_GetITStatus(USART1, USART_IT_RXNE)) // 中断标志
  {
    USART_ClearFlag(USART1,USART_IT_RXNE);
  }
}

void usart2_init(u32 baudRate)
{
  GPIO_InitTypeDef GPIO_InitStrue;
  USART_InitTypeDef USART_InitStrue;
  NVIC_InitTypeDef NVIC_InitStrue;
  // 外设使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  USART_DeInit(USART2);  //复位串口2 -> 可以没有
  // 初始化 串口对应IO口  TX-PA2  RX-PA3
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStrue.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStrue);
  GPIO_InitStrue.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStrue.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStrue);
  // 初始化 串口模式状态
  USART_InitStrue.USART_BaudRate = baudRate; // 波特率
  USART_InitStrue.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制
  USART_InitStrue.USART_Mode = USART_Mode_Tx|USART_Mode_Rx; // 发送 接收 模式都使用
  USART_InitStrue.USART_Parity = USART_Parity_No; // 没有奇偶校验
  USART_InitStrue.USART_StopBits = USART_StopBits_1; // 一位停止位
  USART_InitStrue.USART_WordLength = USART_WordLength_8b; // 每次发送数据宽度为8位
  USART_Init(USART2, &USART_InitStrue);
  USART_Cmd(USART2, ENABLE);//使能串口
  USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);//开启接收中断
  // 初始化 中断优先级
  NVIC_InitStrue.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStrue.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStrue);
}

void USART2_IRQHandler(void) // 串口2中断服务函数
{
  if(USART_GetITStatus(USART2, USART_IT_RXNE)) // 中断标志
  {
    USART_ClearFlag(USART2,USART_IT_RXNE);
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
  usart_send_byte(USARTX, get_char_from_hex(ch));
  ch = value & 0x0F;
  usart_send_byte(USARTX, get_char_from_hex(ch));
  usart_send_byte(USARTX, ' ');
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
    usart_send_byte(USARTX, *(pString++));
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

u8 get_char_from_hex(u8 value)
{
  u8 rt;
  if(value > 9)
  {
    rt = value - 10 + 65;
  }
  else
  {
    rt = value + 48;
  }
  return(rt);
}
