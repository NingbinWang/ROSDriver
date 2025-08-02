/*
 * led.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _LED_H_
#define _LED_H_
#include <stdint.h>
#include "led_conf.h"
#if LED_ENABLE
void GREEN_LEDSET();
void RED_LEDSET();
#endif



#endif /* LED_LED_H_ */
