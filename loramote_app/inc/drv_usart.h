/*
 * drv_usart.h
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#ifndef DRV_USART_H_
#define DRV_USART_H_

#include "stm32f10x.h"

extern void (*USART1_IRQ_call)();

void init_usart();
void usart_init(USART_TypeDef* usartN, GPIO_TypeDef* gpio, u16 txPin, u16 rxPin, u32 baudRate, u32 irqN);
void USART1_IRQHandler();
void usart_send_byte(USART_TypeDef* USARTx, __IO char ch);
void usart_send_u8(u8 value);
void usart_send_u16(u16 value);
void usart_send_u32(u32 value);
void usart_send_string(__IO char* pString);
void usart_send_u8_array(__IO u8* data, u32 length);
void usart_debug(__IO char* s);

#endif /* DRV_USART_H_ */
