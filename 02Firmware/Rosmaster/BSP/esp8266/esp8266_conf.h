#ifndef _ESP8266_CONF_H_
#define _ESP8266_CONF_H_

#include "usart.h"
//需要一个信号量

#define ESP8266_ENBALE  0
#define ESP8266_NOWORK    0

#if (ESP8266_ENBALE)
#define     ESP8266_USART	                          &huart2
#define     ESP8266_USART_BUS	                      USART2
#define		ESP8266_RX_SIZE						      512
#define		ESP8266_RX_FOR_DATA_SIZE			      1024
#define		ESP8266_TX_SIZE							  256
#if INC_FREERTOS_H
#define     ESP8266_Delay                             osDelay
#else
#define     ESP8266_Delay                             HAL_Delay
#endif


#define		ESP8266_WAIT_TIME_LOW					  1000
#define		ESP8266_WAIT_TIME_MED					  10000
#define		ESP8266_WAIT_TIME_HIGH				      25000
#define		ESP8266_WAIT_TIME_VERYHIGH		          60000


#endif



#endif
