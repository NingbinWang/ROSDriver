/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
#include "canworkconf.h"
#if (CANWORK_ENABLE)
#define CAN_ID_H      0x0000
#define CAN_ID_L      0x0000
#define CAN_MASK_H    0x0000
#define CAN_MASK_L    0x0000
#endif
/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */
#if (CANWORK_ENABLE)
	CAN_FilterTypeDef  CAN_sFilterConfig;
	HAL_StatusTypeDef  HAL_Status;
#endif
  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */
#if (CANWORK_ENABLE)
    CAN_sFilterConfig.FilterIdHigh = CAN_ID_H;//
    CAN_sFilterConfig.FilterIdLow = CAN_ID_L;//
    CAN_sFilterConfig.FilterMaskIdHigh = CAN_MASK_H;//
    CAN_sFilterConfig.FilterMaskIdLow = CAN_MASK_L;//
    CAN_sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO1;//
    CAN_sFilterConfig.FilterBank = 0;//
    CAN_sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;//设为IDLIST列表模式/IDMASK屏蔽模式
    CAN_sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;//过滤器位宽度
    CAN_sFilterConfig.FilterActivation = ENABLE;//ENABLE
    CAN_sFilterConfig.SlaveStartFilterBank = 0;//过滤器组设置（单个CAN总线时无用）
    HAL_Status = HAL_CAN_ConfigFilter(canworkhcan, &CAN_sFilterConfig);
    if ( HAL_Status!= HAL_OK)
     {
       /* Filter configuration Error */
       Error_Handler();
     }

     /* Start the CAN peripheral */
    HAL_Status = HAL_CAN_Start(canworkhcan);
     if ( HAL_Status!= HAL_OK)
     {
       /* Start Error */
       Error_Handler();
     }

     /* Activate CAN RX notification */
     //若不使用CAN中断
     HAL_Status = HAL_CAN_ActivateNotification(canworkhcan, CAN_IT_RX_FIFO1_MSG_PENDING);
     if ( HAL_Status!= HAL_OK)
     {
       /* Notification Error */
       Error_Handler();
     }
#endif
  /* USER CODE END CAN1_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN1 GPIO Configuration
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
