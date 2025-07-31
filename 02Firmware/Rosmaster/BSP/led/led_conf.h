/*
 * led_conf.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _LED_CONF_H_
#define _LED_CONF_H_
#define LED_ENABLE 1
#if LED_ENABLE
#include "gpio.h"
#define LED1_GREEN_Pin                   GPIO_PIN_14
#define LED1_GREEN_GPIO_Port             GPIOE
#define LED2_RED_Pin                     GPIO_PIN_1
#define LED2_RED_GPIO_Port               GPIOA
#endif



#endif /* LED_LED_CONF_H_ */
