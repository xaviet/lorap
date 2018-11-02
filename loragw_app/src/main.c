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
#include "drv_timer.h"

int main(void)
{
  init();
  loop();
}

void init()
{
  // 外设使能时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2
			 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
//			 RCC_APB2Periph_GPIOB |
			 RCC_APB2Periph_USART1 |
			 RCC_APB2Periph_TIM1
			 , ENABLE);
  init_usart();
  init_global();
  init_gpio();
  init_flash();
  init_timer();
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

