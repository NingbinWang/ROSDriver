/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TRIG_Pin GPIO_PIN_0
#define TRIG_GPIO_Port GPIOC
#define ECHO_Pin GPIO_PIN_1
#define ECHO_GPIO_Port GPIOC
#define PWMA_Pin GPIO_PIN_0
#define PWMA_GPIO_Port GPIOA
#define REDLED_Pin GPIO_PIN_1
#define REDLED_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_4
#define KEY2_GPIO_Port GPIOA
#define PWR_ADC_Pin GPIO_PIN_4
#define PWR_ADC_GPIO_Port GPIOC
#define AIN1_Pin GPIO_PIN_5
#define AIN1_GPIO_Port GPIOC
#define AIN2_Pin GPIO_PIN_2
#define AIN2_GPIO_Port GPIOB
#define BIN1_Pin GPIO_PIN_7
#define BIN1_GPIO_Port GPIOE
#define BIN2_Pin GPIO_PIN_8
#define BIN2_GPIO_Port GPIOE
#define PWMB_Pin GPIO_PIN_9
#define PWMB_GPIO_Port GPIOE
#define VGA_LED_Pin GPIO_PIN_10
#define VGA_LED_GPIO_Port GPIOE
#define VGA_DC_Pin GPIO_PIN_11
#define VGA_DC_GPIO_Port GPIOE
#define VGA_RESET_Pin GPIO_PIN_12
#define VGA_RESET_GPIO_Port GPIOE
#define VGA_CS_Pin GPIO_PIN_13
#define VGA_CS_GPIO_Port GPIOE
#define GREEDLED_Pin GPIO_PIN_14
#define GREEDLED_GPIO_Port GPIOE
#define FLASH_CS_Pin GPIO_PIN_15
#define FLASH_CS_GPIO_Port GPIOE
#define E1A_Pin GPIO_PIN_14
#define E1A_GPIO_Port GPIOB
#define E1B_Pin GPIO_PIN_15
#define E1B_GPIO_Port GPIOB
#define E2A_Pin GPIO_PIN_6
#define E2A_GPIO_Port GPIOC
#define E2B_Pin GPIO_PIN_7
#define E2B_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_15
#define KEY1_GPIO_Port GPIOA
#define STBY_Pin GPIO_PIN_3
#define STBY_GPIO_Port GPIOD
#define NRF_CS_Pin GPIO_PIN_5
#define NRF_CS_GPIO_Port GPIOD
#define NRF_CE_Pin GPIO_PIN_6
#define NRF_CE_GPIO_Port GPIOD
#define NRF_IRQ_Pin GPIO_PIN_7
#define NRF_IRQ_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
