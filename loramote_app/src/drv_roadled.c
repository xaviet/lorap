/*
 * drv_roadled.c
 *
 *  Created on: Nov 9, 2018
 *      Author: mose
 */

#include "config.h"
#include "drv_roadled.h"
#include "drv_gpio.h"

void init_roadled()
{
  gpio_init(ROADLED_PIN, ROADLED_GPIO, GPIO_Speed_2MHz, GPIO_Mode_Out_PP);
  gpio_init(LEDSENSOR_PIN, LEDSENSOR_GPIO, GPIO_Speed_2MHz, GPIO_Mode_IPU);
}

void roadled_set(u8 s)
{
  if(s)
  {
    gpio_set(ROADLED_GPIO, ROADLED_PIN, ON);
  }
  else
  {
    gpio_set(ROADLED_GPIO, ROADLED_PIN, OFF);
  }
}

u8 roadled_sensor_get()
{
  return(gpio_get(LEDSENSOR_GPIO, LEDSENSOR_PIN));
}
