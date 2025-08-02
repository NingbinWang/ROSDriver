/*
 * led.c
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#include "led_conf.h"
#if LED_ENABLE

void GREEN_LEDSET()
{
	HAL_GPIO_TogglePin(LED1_GREEN_GPIO_Port,LED1_GREEN_Pin);
}


void RED_LEDSET()
{
	HAL_GPIO_TogglePin(LED2_RED_GPIO_Port,LED2_RED_Pin);

}



#endif
