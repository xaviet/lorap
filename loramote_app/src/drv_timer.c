/*
 * drv_timer.c
 *
 *  Created on: Nov 2, 2018
 *      Author: mose
 */

#include "config.h"
#include "drv_timer.h"
#include "drv_usart.h"
#include "tools_lib.h"

void (*TIM1_IRQ_call)();
void (*TIM2_IRQ_call)();

void init_timer()
{
  TIM1_IRQ_call = FALSE;
  TIM2_IRQ_call = FALSE;
  timer_init(TIM1, 2, 36000, TIM_CKD_DIV1, TIM_CounterMode_Up, 0, TIM1_UP_IRQn);
  timer_init(TIM2, 2000, 36000, TIM_CKD_DIV1, TIM_CounterMode_Up, 0, TIM2_IRQn);
}

void timer_init(TIM_TypeDef* timN, u16 period, u16 prescaler, u16 clockDivision, u16 countMode, u8 repetition, u8 irqChannel)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Period = period-1; //设置自动重装载寄存器周期的值 2 * 0.5ms = 1ms
  TIM_TimeBaseStructure.TIM_Prescaler =prescaler-1; //设置时钟频率除数的预分频值 36000 / 72MHz = 0.5ms
  TIM_TimeBaseStructure.TIM_ClockDivision = clockDivision; //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = countMode; //TIM 向上计数
  TIM_TimeBaseStructure.TIM_RepetitionCounter = repetition; //重复计数，重复溢出多少次,一个溢出中断
  TIM_TimeBaseInit(timN, &TIM_TimeBaseStructure);
  //允许更新中断
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ITConfig(timN,TIM_IT_Update,ENABLE );
  //中断优先级 NVIC 设置
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = irqChannel; //TIM 中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //先占优先级 2 级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级 2 级
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
  NVIC_Init(&NVIC_InitStructure); //初始化 NVIC 寄存器
  TIM_Cmd(timN, ENABLE); //使能 TIM1
}

void TIM1_UP_IRQHandler()
{
  if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) //检查 TIM1 更新中断发生与否
  {
    TIM_ClearFlag(TIM1, TIM_IT_Update ); //清除 TIM1 更新中断标志
    call_fun(TIM1_IRQ_call);
  }
}

void TIM2_IRQHandler()
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) //检查 TIM1 更新中断发生与否
   {
     TIM_ClearFlag(TIM2, TIM_IT_Update ); //清除 TIM1 更新中断标志
     call_fun(TIM2_IRQ_call);
//     usart_send_string("timer2\r\n");
   }
}

//#define SYSCLK_FREQ_72MHz  72000000
//#define PRESCALER 72
//#define SERVOPERIOD 50
//void timer4Init()
//{
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//  GPIO_InitTypeDef GPIO_InitStructure;   //定义结构体
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  TIM_TimeBaseStructure.TIM_Prescaler = (u16)(PRESCALER - 1); // 72
//  TIM_TimeBaseStructure.TIM_Period = (u16)((SYSCLK_FREQ_72MHz / PRESCALER / SERVOPERIOD) - 1); // 20000
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//  TIM_OCInitTypeDef TIM_OCInitStructure;
//  TIM_OCStructInit(&TIM_OCInitStructure);
//  TIM_OCInitStructure.TIM_Pulse = 1000;
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//  TIM_Cmd(TIM4, ENABLE);
//  TIM_SetCompare4(TIM4, 700); // 500: -90/1500: 0/2500: 90
//  delay_1us(1000000);
//  TIM_SetCompare4(TIM4, 2300);
////  GPIO_PinRemapConfig();
//}
