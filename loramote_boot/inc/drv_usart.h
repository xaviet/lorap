/*
 * drv_usart.h
 *
 *  Created on: Aug 22, 2018
 *      Author: pv
 */

#ifndef DRV_USART_H_
#define DRV_USART_H_

#include "stm32f10x.h"

#define USARTX USART1

void (*USART1_IRQ_call)();
void (*USART2_IRQ_call)();

void usart_init();
void usart1_init(u32 baudRate);
void USART1_IRQHandler();
void usart2_init(u32 baudRate);
void USART2_IRQHandler();
void usart_send_byte(USART_TypeDef* USARTx, char ch);
void usart_send_u8(u8 value);
void usart_send_u16(u16 value);
void usart_send_u32(u32 value);
void usart_send_string(char* pString);
void usart_send_u8_array(u8* data, u32 length);
u8 get_char_from_hex(u8 value);

#endif /* DRV_USART_H_ */
