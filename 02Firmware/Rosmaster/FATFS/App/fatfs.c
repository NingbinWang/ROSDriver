/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
 /*
  if(retSD != 0) return FR_NOT_READY;

     // 挂载文件系统
   FRESULT res = f_mount(&fs, path, 1);
   if(res != FR_OK) return res;

     // 获取SD卡信息
   DWORD free_clusters, total_sectors, free_sectors;
   FATFS* fs_ptr;

   res = f_getfree(path, &free_clusters, &fs_ptr);
   if(res != FR_OK) return res;

     // 计算容量信息
   total_sectors = (fs_ptr->n_fatent - 2) * fs_ptr->csize;
   free_sectors = free_clusters * fs_ptr->csize;

     printf("total size : %.2f MB\r\n", (total_sectors / 2) / 1024.0);
     printf("free size: %.2f MB\r\n", (free_sectors / 2) / 1024.0);
     */
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
