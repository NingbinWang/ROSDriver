/*
 * key_conf.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _KEY_CONF_H_
#define _KEY_CONF_H_

#define KEY_ENABLE 1
#if KEY_ENABLE
#include "gpio.h"
#define BUTTON1_Pin                    GPIO_PIN_15
#define BUTTON1_GPIO_Port              GPIOA
#define BUTTON1_STATUS_ON              GPIO_PIN_RESET
#define BUTTON2_Pin                   GPIO_PIN_4
#define BUTTON2_GPIO_Port             GPIOA
#define BUTTON2_STATUS_ON              GPIO_PIN_RESET
#endif

#endif /*_KEY_CONF_H_ */
