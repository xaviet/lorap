/*
 * drv_itemp.c
 *
 *  Created on: Nov 3, 2018
 *      Author: mose
 */

#include "drv_itemp.h"
#include "drv_gpio.h"
#include "drv_usart.h"

void init_iTemp()
{
  RCC_ADCCLKConfig(RCC_PCLK2_Div8);   //分频因子6时钟为72M/6=12MHz
  ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //ADC工作模式:ADC1和ADC2工作在独立模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;   //模数转换工作在单通道模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //模数转换工作在单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //转换由软件而不是外部触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel = 1; //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);  //ADC1,ADC通道16,第一个转换,采样时间为239.5周期
  ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
  ADC_Cmd(ADC1, ENABLE);  //使能指定的ADC1
  ADC_ResetCalibration(ADC1); //重置指定的ADC1的复位寄存器
  while(ADC_GetResetCalibrationStatus(ADC1)); //获取ADC1重置校准寄存器的状态,设置状态则等待
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));      //获取指定ADC1的校准程序,设置状态则等待
  usart_send_string("\tiTemp: ");
  usart_send_u16((u16)iTemp_get(8));
  usart_send_string("\r\n");
}

vs16 iTemp_get(vu8 times)
{
  u16 min = 0xffff;
  u16 max = 0x0000;
  s32 vTemp = 0;
  s16 v = 0;
  for(int i = 0; i < times; i++)
  {
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能指定的ADC1的软件转换启动功能
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )); //等待转换结束
    v = ADC_GetConversionValue(ADC1);
    vTemp += v;
    if(v > max)
    {
      max = v;
    }
    if(v < min)
    {
      min = v;
    }
  }
  vTemp = (vTemp - min - max) / (times - 2);
  vTemp = (vTemp * (3300000 / 32)) / 128; // uV
  vTemp = (1430000 - vTemp) / 43 + 2500; // unit: 0.01
  return((vs16)(vTemp & 0x0000ffff));    //返回最近一次ADC1规则组的转换结果
}
