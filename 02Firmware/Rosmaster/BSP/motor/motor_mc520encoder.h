/*
 * motor_mc520encoder.h
 *
 *  Created on: Jul 30, 2025
 *      Author: Administrator
 */

#ifndef _MOTOR_MC520ENCODER_H_
#define _MOTOR_MC520ENCODER_H_
#include "motorconf.h"
#if MOTORMC520_ENABLE
#include <stdint.h>
#include "tim.h"
void SetMotorMC520AEncoderCount(int16_t count);
void SetMotorMC520BEncoderCount(int16_t count);
int GetMotorAEncoderCount(void);
int GetMotorBEncoderCount(void);
void MotorAEncoderInit(void);
void MotorBEncoderInit(void);
#endif

#endif /* MOTOR_MOTOR_MC520ENCODER_H_ */
