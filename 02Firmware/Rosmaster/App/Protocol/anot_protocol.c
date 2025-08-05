#include <stdio.h>
#include "config.h"
#if ANOT_PROTOCOL_ENABLE
#include "anot_protocol.h"
#include <string.h>
#include <stdint.h>


bool ANO_Protocol_SendPacket(uint8_t *pbuf, uint8_t len,const ANTO_MODE_E mode)
{
	bool ret;
    uint8_t sumcheck;
    uint8_t addcheck;
	uint8_t databuf[len+6];
	int i = 0;
   databuf[0] = 0xAA;
   databuf[1] = 0xff;
   databuf[2] = mode;
   databuf[3] = len;
   for(i = 0;i < len;i++)
   {
	   databuf[4+i]=pbuf[i];
   }
   for(i=0;i<(len+4);i++){
	   sumcheck += databuf[i];
	   addcheck += sumcheck;
   }
   databuf[4+len] = sumcheck;
   databuf[5+len] = addcheck;
#if ANOT_TRANSF_ESP8266
   ret = esp8266_TcpIp_SendDataUdp(0,len+6,(uint8_t*)databuf);
#elif ANOT_TRANSF_UART
  if(HAL_UART_Transmit(ANOProtocolUart,(uint8_t *)databuf,len+6,0xffff) != HAL_OK)
	{
		 Error_Handler();
		 ret = FALSE;
	}
   ret  = TRUE;	 
#endif
   return ret;

}

int ANO_Protocol_RecivePacket(uint8_t *pbuf, uint8_t len)
{
    return 0;
}





#endif

