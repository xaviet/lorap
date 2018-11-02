/*
 * main.c
 *
 *  Created on: Nov 1, 2018
 *      Author: mose
 */

#include "main.h"
#include "drv_usart.h"
#include "drv_flash.h"
#include "drv_gpio.h"

int main(void)
{
  init();
  loop();
}

void init()
{
  // 外设使能时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
//			 RCC_APB2Periph_GPIOB |
			 RCC_APB2Periph_USART1
			 , ENABLE);
  init_global();
  init_gpio();
  init_usart();
  init_flash();

  usart_send_string("\r\n\tApp\r\n");
}

void loop()
{
  while(TRUE)
  {
    led_set(ON);
    delay_1us(0x80000);
    led_set(OFF);
    delay_1us(0x80000);
  }
}

