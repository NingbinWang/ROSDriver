#ifndef _CANWORK_H_
#define _CANWORK_H_
#include "can.h"


extern CAN_HandleTypeDef hcan1;//声明的HAL库结构体

#define CAN1_REC_LEN  256//定义CAN1最大接收字节数

extern uint8_t  CAN1_RX_BUF[CAN1_REC_LEN];//接收缓冲,末字节为换行符
extern uint16_t CAN1_RX_STA;//接收状态标记

uint8_t  CAN1_SendNormalData(CAN_HandleTypeDef*  hcan,uint16_t ID,uint8_t *pData,uint16_t  Len);//CAN发送函数
void CAN1_printf (char *fmt, ...);//CAN总线通信，使用CAN1，这是CAN专用的printf函数


#endif
