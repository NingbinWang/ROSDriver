#include <stdio.h>
#include "config.h"

#if AX_PROTOCOL_ENABEL
#include "ax_protocol.h"

#define MAX_PROTOCOL_LEN 40
#define AX_PROTOCOL_ERR -1
#define AX_PROTOCOL_OK 0
/**
  * @简  述  UART 发送数据（X-Protocol协议）
  * @参  数  *pbuf：发送数据指针
  *          len：发送数据长度个数，≤27 (32-5)
  *          num：帧号，帧编码
  * @返回值	 无
  */
int AX_Protocol_SendPacket(uint8_t *pbuf, uint8_t len, uint8_t num)
{
	uint8_t i,cnt;
 	uint8_t tx_checksum = 0;//发送校验和
 	uint8_t axprotocolbuf[MAX_PROTOCOL_LEN];
	if(len > 32)
		return AX_PROTOCOL_ERR;
	//获取数
	axprotocolbuf[0] = 0xAA;    //帧头
	axprotocolbuf[1] = 0x55;    //
	axprotocolbuf[2] = len+5;  //根据输出长度计算帧长度
	axprotocolbuf[3] = num;    //帧编码
	for(i=0; i<len; i++)
	{
		axprotocolbuf[4+i] = *(pbuf+i);
	}

	//计算校验和
	cnt = 4+len;
	for(i=0; i<cnt; i++)
	{
		tx_checksum = tx_checksum + axprotocolbuf[i];
	}
	axprotocolbuf[i] = tx_checksum;
	//发送数据
	cnt = 5+len;
	//查询传输方式
	if(HAL_UART_Transmit(AXProtocolUart,(uint8_t *)axprotocolbuf,len,0xffff) != HAL_OK)
		return AX_PROTOCOL_ERR;
	return AX_PROTOCOL_OK;
}



/**
  * @简  述  DBUART 串口中断服务函数
  * @参  数  无
  * @返回值  无
  */
/*
void ProtocolUartBusNUM_IRQHandler(void)
{
	static uint8_t uart_db_flag_rx_ok = 0; //接收成功标志
	static uint8_t uart_db_rx_con=0;	   //接收计数器
	static uint8_t uart_db_rx_checksum;    //帧头部分校验和
	static uint8_t uart_db_rx_buf[40];	   //接收缓冲，数据内容小于等于32Byte

	uint8_t Res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res =USART_ReceiveData(USART1);

		if(uart_db_rx_con < 3)    //==接收帧头 + 长度
		{
			if(uart_db_rx_con == 0)  //接收帧头1 0xAA
			{
				if(Res == 0xAA)
				{
					uart_db_rx_buf[0] = Res;
					uart_db_rx_con = 1;
				}
				else
				{

				}
			}else if(uart_db_rx_con == 1) //接收帧头2 0x55
			{
				if(Res == 0x55)
				{
					uart_db_rx_buf[1] = Res;
					uart_db_rx_con = 2;
				}
				else
				{
					uart_db_rx_con = 0;
				}
			}
			else  //接收数据长度
			{
				uart_db_rx_buf[2] = Res;
				uart_db_rx_con = 3;
				uart_db_rx_checksum = (0xAA+0x55) + Res;	//计算校验和
			}
		}
		else    //==接收数据
		{
			if(uart_db_rx_con < (uart_db_rx_buf[2]-1) )
			{
				uart_db_rx_buf[uart_db_rx_con] = Res;
				uart_db_rx_con++;
				uart_db_rx_checksum = uart_db_rx_checksum + Res;
			}
			else    //判断最后1位
			{
				//数据校验
				if( Res == uart_db_rx_checksum )  //校验正确
				{
					//此处进行数据解析
					uart_db_flag_rx_ok = 1;

					//接收完成，恢复初始状态
					uart_db_rx_con = 0;
				}
			}
		}

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	}
}
*/
/**
  * @简  述  获取接收的数据
  * @参  数  *pbuf：接收数据指针,第1个字节为帧编码，后面为数据
  * @返回值	 0-无数据接收，other-需要读取的数据字节个数
  */
uint8_t AX_Protocol_GetRxData(uint8_t *pbuf)
{
		return 0;

}


#endif
