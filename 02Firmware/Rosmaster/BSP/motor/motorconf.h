/*
 * motorconf.h
 *
 *  Created on: Apr 15, 2025
 *      Author: Administrator
 */

#ifndef MOTORCONF_H_
#define MOTORCONF_H_

#define MOTORGM37_ENABLE 0
#if MOTORGM37_ENABLE
#include "tim.h"
#include "gpio.h"
#define MOTORGM37A_PWM                        &htim1
#define MOTORGM37A_PWM_CH                     TIM_CHANNEL_1
#define MOTORGM37B_PWM                        &htim2
#define MOTORGM37B_PWM_CH                     TIM_CHANNEL_1

#define MOTORGM37A_AIN2_Pin                   GPIO_PIN_7
#define MOTORGM37A_AIN2_GPIO_Port             GPIOE
#define MOTORGM37A_AIN1_Pin                   GPIO_PIN_8
#define MOTORGM37A_AIN1_GPIO_Port             GPIOE
#define MOTORGM37B_BIN1_Pin                   GPIO_PIN_1
#define MOTORGM37B_BIN1_GPIO_Port             GPIOA
#define MOTORGM37B_BIN2_Pin                   GPIO_PIN_4
#define MOTORGM37B_BIN2_GPIO_Port             GPIOA



#define MOTORGM37_HALLENABLE 0
#if MOTORGM37_HALLENABLE
#define MOTORGM37A_HALL_GPIO_Port            GPIOA
#define MOTORGM37A_HALL_Pin                  GPIO_PIN_7

#define MOTORGM37B_HALL_GPIO_Port            GPIOB
#define MOTORGM37B_HALL_Pin                  GPIO_PIN_1

#define MOTORGM37A_TIMER                     &htim3
#define MOTORGM37B_TIMER                     &htim3

#define MOTORGM37A_EXT_TIMER                 TIM3
#define MOTORGM37A_EXT_CH                    TIM_CHANNEL_1
#define MOTORGM37A_EXT_ACVITE_CH             HAL_TIM_ACTIVE_CHANNEL_1

#define MOTORGM37B_EXT_TIMER                 TIM3
#define MOTORGM37B_EXT_CH                    TIM_CHANNEL_2
#define MOTORGM37B_EXT_ACVITE_CH             HAL_TIM_ACTIVE_CHANNEL_2
#endif
#endif



#define MOTORMC520_ENABLE 1
#if MOTORMC520_ENABLE
#include "tim.h"
#include "gpio.h"

#define MOTORMC520A_PWM                        &htim8
#define MOTORMC520A_PWM_CH                     TIM_CHANNEL_1
#define MOTORMC520B_PWM                        &htim1
#define MOTORMC520B_PWM_CH                     TIM_CHANNEL_1

#define MOTORMC520A_AIN1_Pin                   GPIO_PIN_5
#define MOTORMC520A_AIN1_GPIO_Port             GPIOC
#define MOTORMC520A_AIN2_Pin                   GPIO_PIN_2
#define MOTORMC520A_AIN2_GPIO_Port             GPIOB
#define MOTORMC520B_BIN1_Pin                   GPIO_PIN_7
#define MOTORMC520B_BIN1_GPIO_Port             GPIOE
#define MOTORMC520B_BIN2_Pin                   GPIO_PIN_8
#define MOTORMC520B_BIN2_GPIO_Port             GPIOE

#define MOTORMC520_ENCODERENABLE 1
#if MOTORMC520_ENCODERENABLE

#define MOTORMC520A_TIMER                     &htim2
#define MOTORMC520A_EXT_TIMER                 TIM2

#define MOTORMC520B_TIMER                     &htim3
#define MOTORMC520B_EXT_TIMER                 TIM3

#endif

#endif

#endif /* MOTORCONF_H_ */
