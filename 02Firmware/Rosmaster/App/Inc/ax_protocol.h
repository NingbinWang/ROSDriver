#ifndef _AX_PROTOCOL_H_
#define _AX_PROTOCOL_H_

#include "config.h"
#include <stdint.h>
#if AX_PROTOCOL_ENABEL
#define AXProtocolUart &huart3
#define AXProtocolUartBus USART3



int AX_Protocol_SendPacket(uint8_t *pbuf, uint8_t len, uint8_t num);



#endif





#endif
