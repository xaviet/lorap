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
#include "drv_spi.h"
#include "drv_sx1278.h"
#include "drv_w5500.h"
#include "drv_itemp.h"
#include "drv_iwdg.h"
#include "drv_exti.h"
#include "task.h"

int main(void)
{
  init();
  loop();
}

void init()
{
  // 外设使能时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 |
			 RCC_APB1Periph_TIM2
			 , ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
			 RCC_APB2Periph_GPIOB |
			 RCC_APB2Periph_GPIOC |
			 RCC_APB2Periph_USART1 |
			 RCC_APB2Periph_SPI1 |
			 RCC_APB2Periph_ADC1 |
			 RCC_APB2Periph_TIM1 |
			 RCC_APB2Periph_AFIO
			 , ENABLE);
  init_flash();
  init_gpio();
  init_usart();
  init_global();
  init_timer();
  init_spi();
  init_sx1278();
  init_w5500();
  init_iTemp();
  init_iwdg();
  init_exti();
  init_task();
  usart_send_string("\tLoRaGW running\r\n");
  usart_send_string("\tCompile: ");
  usart_send_string(__DATE__);
  usart_send_string(" ");
  usart_send_string(__TIME__);
  usart_send_string("\r\n");
}

void loop()
{
  while(TRUE)
  {
    iwdg_feed();
    task_run();
  }
}

