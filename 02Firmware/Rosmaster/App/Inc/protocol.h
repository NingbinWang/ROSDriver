/*
 * protocol.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#define RXBUFFERSIZE  512     //最大接收字节数

typedef struct{
	char RxBuffer[RXBUFFERSIZE];   //接收数据
	uint16_t uart_cnt;

}PROTOCOL_UART_T;
#if AX_PROTOCOL_ENABEL
#include "ax_protocol.h"
#endif

#endif /* INC_PROTOCOL_H_ */
