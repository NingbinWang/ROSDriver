/*
 * motor_mc520.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Administrator
 */

#include "motorconf.h"
#if MOTORMC520_ENABLE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "motor_mc520.h"

/*
*********************************************************************************************************
*	函 数 名: MotorA_Forward
*	功能说明: 电机A正转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520A_Forward(void)
{
    HAL_GPIO_WritePin(MOTORMC520A_AIN1_GPIO_Port, MOTORMC520A_AIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORMC520A_AIN2_GPIO_Port, MOTORMC520A_AIN2_Pin, GPIO_PIN_SET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorA_Forward
*	功能说明: 电机A反转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520A_Backward(void)
{
    HAL_GPIO_WritePin(MOTORMC520A_AIN1_GPIO_Port, MOTORMC520A_AIN1_Pin, GPIO_PIN_SET);

    HAL_GPIO_WritePin(MOTORMC520A_AIN2_GPIO_Port, MOTORMC520A_AIN2_Pin, GPIO_PIN_RESET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorA_Stop
*	功能说明: 电机A停止
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520A_Stop(void)
{
    HAL_GPIO_WritePin(MOTORMC520A_AIN1_GPIO_Port, MOTORMC520A_AIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORMC520A_AIN2_GPIO_Port, MOTORMC520A_AIN2_Pin, GPIO_PIN_RESET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorB_Forward
*	功能说明: 电机B正转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520B_Forward(void)
{
    HAL_GPIO_WritePin(MOTORMC520B_BIN1_GPIO_Port, MOTORMC520B_BIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORMC520B_BIN2_GPIO_Port, MOTORMC520B_BIN2_Pin, GPIO_PIN_SET);

}
/*
*********************************************************************************************************
*	函 数 名: MotorB_Backward
*	功能说明: 电机B反转
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520B_Backward(void)
{
    HAL_GPIO_WritePin(MOTORMC520B_BIN1_GPIO_Port, MOTORMC520B_BIN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTORMC520B_BIN2_GPIO_Port, MOTORMC520B_BIN2_Pin, GPIO_PIN_RESET);
}


/*
*********************************************************************************************************
*	函 数 名: MotorA_Stop
*	功能说明: 电机A停止
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void MotorMC520B_Stop(void)
{
    HAL_GPIO_WritePin(MOTORMC520B_BIN1_GPIO_Port, MOTORMC520B_BIN1_Pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(MOTORMC520B_BIN2_GPIO_Port, MOTORMC520B_BIN2_Pin, GPIO_PIN_RESET);

}

/*
*********************************************************************************************************
*	函 数 名: MotoASetSpeed
*	功能说明: A电机的占空比设置
*	形    参: _direction ：运动方向 00表示正转 01表示反转 _speed：占空比值 范围0~2000
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorMC520ASetSpeed(int8_t _direction, int16_t _speed)
{
    if(_direction == 0)
    {

    	MotorMC520A_Forward();
        __HAL_TIM_SET_COMPARE(MOTORMC520A_PWM, MOTORMC520A_PWM_CH, abs(_speed));
    }
    else
    {
    	MotorMC520A_Backward();
        __HAL_TIM_SET_COMPARE(MOTORMC520A_PWM, MOTORMC520A_PWM_CH, abs(_speed));
    }
    HAL_TIM_PWM_Start(MOTORMC520A_PWM, MOTORMC520A_PWM_CH);
}

void MotorMC520APWMStart()
{
    HAL_TIM_PWM_Start(MOTORMC520A_PWM, MOTORMC520A_PWM_CH);
}


/*
*********************************************************************************************************
*	函 数 名: MotoBSetSpeed
*	功能说明: B电机的占空比设置
*	形    参: _direction ：运动方向 00表示正转 01表示反转 _speed：占空比值 范围0~2000
*	返 回 值: 无
*********************************************************************************************************
*/

void MotorMC520BSetSpeed(int8_t _direction, int16_t _speed)
{
    if(_direction == 0)
    {
    	MotorMC520B_Forward();

        __HAL_TIM_SET_COMPARE(MOTORMC520B_PWM, MOTORMC520B_PWM_CH, abs(_speed));

    }
    else
    {
    	MotorMC520B_Backward();
        __HAL_TIM_SET_COMPARE(MOTORMC520B_PWM, MOTORMC520B_PWM_CH, abs(_speed));
    }
    HAL_TIM_PWM_Start(MOTORMC520B_PWM, MOTORMC520B_PWM_CH);

}



void MotorMC520BPWMStart()
{
    HAL_TIM_PWM_Start(MOTORMC520B_PWM, MOTORMC520B_PWM_CH);
}


#endif
