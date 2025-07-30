/*
 * motor_mc520encoder.c
 *
 *  Created on: Jul 30, 2025
 *      Author: Administrator
 */
#include "motorconf.h"
#if MOTORMC520_ENCODERENABLE
#include "motor_mc520encoder.h"
void MotorAEncoderInit(void)
{
	HAL_TIM_Encoder_Start(MOTORMC520A_TIMER, TIM_CHANNEL_ALL);
}

void MotorBEncoderInit(void)
{
	HAL_TIM_Encoder_Start(MOTORMC520B_TIMER, TIM_CHANNEL_ALL);
}



int GetMotorAEncoder(void)//获取motorA 编码器脉冲
{
	int count = 0;
	int direct = 0;
	direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(MOTORMC520A_TIMER);
	count = (short)(__HAL_TIM_GET_COUNTER(MOTORMC520A_TIMER));//先读取脉冲数
    __HAL_TIM_SET_COUNTER(MOTORMC520A_TIMER,0);//再计数器清零
    return count;//返回脉冲数
}

int GetMotorBEncoder(void)//获取motorB 编码器脉冲
{
	int count = 0;
	int direct = 0;
	direct = __HAL_TIM_IS_TIM_COUNTING_DOWN(MOTORMC520B_TIMER);
	count = (short)(__HAL_TIM_GET_COUNTER(MOTORMC520B_TIMER));//先读取脉冲数
    __HAL_TIM_SET_COUNTER(MOTORMC520B_TIMER,0);//再计数器清零
    return count;//返回脉冲数
}
#endif
