/*
 * key.c
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */
#include "key_conf.h"
#if KEY_ENABLE

#include "key.h"
#include <string.h>
BUTTON_T button = {0};

int iButtonCount;//i代表int型变量，ButtonCount表示按键计数变量
int iButtonFlag;//i代表int型变量，ButtonFlag表示重按键标志，1代表重新按键，0为没有重新按键

void ButtonScan(void)
{
  if(HAL_GPIO_ReadPin(BUTTON1_GPIO_Port,BUTTON1_Pin) == GPIO_PIN_RESET )//如果引脚检测到低电平
  {
      iButtonCount++;                         //按键按下，计数iButtonCount加1
      if(iButtonCount>=30)                    //1ms中断服务函数里运行一次，iButtonCount大于等于30，即按键已稳定按下30ms
      {
           if(iButtonFlag==0)                  //判断有没有重按键，1为有，0为没有
           {
                button.key1=1;                 //设置按键标志
                iButtonCount=0;
                iButtonFlag=1;                  //设置重按键标志
           }
           else                              //如果重按键，则重新计数
              iButtonCount=0;
     }
     else                                  //如果没有稳定按下30ms，则代表没有按下按键
         button.key1=0;

  }else if(HAL_GPIO_ReadPin(BUTTON2_GPIO_Port,BUTTON2_Pin) == GPIO_PIN_RESET){

   	iButtonCount++;						   //按键按下，计数iButtonCount加1
    if(iButtonCount>=30) 				   //1ms中断服务函数里运行一次，iButtonCount大于等于30，即按键已稳定按下30ms
    {
		if(iButtonFlag==0)					//判断有没有重按键，1为有，0为没有
		{
			 button.key2=1; 				//设置按键标志
			 iButtonCount=0;
			 iButtonFlag=1; 				 //设置重按键标志
		}
		else							  //如果重按键，则重新计数
		   iButtonCount=0;
   }
   else									//如果没有稳定按下30ms，则代表没有按下按键
	      button.key2=0;
  }else                                      //如果一直无检测到低电平，即一直无按键按下
  {
         iButtonCount=0;                  //清零iButtonCount
         button.key1=0;
		 button.key2=0;
         iButtonFlag=0;                   //清除重按键标志
  }
}

void GetButton_status(BUTTON_T* btn)
{
	memcpy((void*)btn,(void *)&button,sizeof(BUTTON_T));
}




#endif


