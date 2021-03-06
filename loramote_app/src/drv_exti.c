/*
 * drv_exti.c
 *
 *  Created on: Nov 4, 2018
 *      Author: mose
 */

#include "hw_config.h"
#include "config.h"
#include "drv_exti.h"
#include "drv_usart.h"
#include "drv_gpio.h"
#include "tools_lib.h"

void (*sx1278_dio0_IRQ_call)();
//void (*w5500_int_IRQ_call)();

void init_exti()
{
  sx1278_dio0_IRQ_call = FALSE;
//  w5500_int_IRQ_call = FALSE;
  // reboot_button
//  exti_init(BUTTON_PIN, BUTTON_GPIO, EXTI_Line0, GPIO_PortSourceGPIOA, GPIO_PinSource0, EXTI0_IRQn, EXTI_Trigger_Rising_Falling);
  // sx1278_dio0
  exti_init(SX1278_DOI0, SX1278_DIO0_GPIO, EXTI_Line0, GPIO_PortSourceGPIOB, GPIO_PinSource0, EXTI0_IRQn, EXTI_Trigger_Rising);
  // w5500_int
//  exti_init(W5500_INT, W5500_INT_GPIO, EXTI_Line7, GPIO_PortSourceGPIOC, GPIO_PinSource7, EXTI9_5_IRQn, EXTI_Trigger_Falling);

}

void exti_init(u16 spiN, GPIO_TypeDef* gpio, u32 extiLine, u8 portSource, u8 pinSource, u8 channel, EXTITrigger_TypeDef trigger)
{
  gpio_init(spiN, gpio, GPIO_Speed_2MHz, GPIO_Mode_IPD);
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_ClearITPendingBit(extiLine); //PB0
  GPIO_EXTILineConfig(portSource, pinSource);//PB0  为GPIOB的PIN3
  EXTI_InitStructure.EXTI_Line= extiLine; //PB0，为：EXTI_Line3
  EXTI_InitStructure.EXTI_Mode= EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger= trigger;   //中断方式为上升 or下降沿
  EXTI_InitStructure.EXTI_LineCmd=ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //NVIC
  NVIC_InitStructure.NVIC_IRQChannel = channel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority= 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

//void EXTI0_IRQHandler()
//{
//  EXTI_ClearITPendingBit(EXTI_Line0);
//  if(gpio_get(BUTTON_GPIO, BUTTON_PIN) == 0x00)
//  {
//    usart_send_string("Falling\r\n");
//    globalV.rebootPressedTime;
//  }
//  else
//  {
//    usart_send_string("Rising\r\n");
//  }
//}

// sx1278 irq
void EXTI0_IRQHandler()
{
  EXTI_ClearITPendingBit(EXTI_Line0);
  call_fun(sx1278_dio0_IRQ_call);
}

// w5500 irq
//void EXTI9_5_IRQHandler()
//{
//  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
//  {
//    EXTI_ClearITPendingBit(EXTI_Line7);
//    call_fun(w5500_int_IRQ_call);
//  }
//}
