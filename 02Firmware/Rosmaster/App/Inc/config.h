#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "usart.h"
#include <stdio.h>
// printf uart bus
#define PRINTF_ENBALE 1
#if PRINTF_ENBALE
#define PRINTFUart &huart1
#define PRINTFUartBus USART1
#endif

#define Debug_Level 0

#define AX_PROTOCOL_ENABEL               0
#define ANOT_PROTOCOL_ENABLE              1
#define SYS_COMMANDLINE_ENABLE           1


#endif
