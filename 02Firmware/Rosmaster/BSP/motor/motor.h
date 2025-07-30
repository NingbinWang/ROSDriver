/*
 * motor.h
 *
 *  Created on: Jul 30, 2025
 *      Author: Administrator
 */

#ifndef MOTOR_H_
#define MOTOR_H_
#include "motorconf.h"
#if MOTORMC520_ENABLE
#include "motor_mc520.h"
#include "motor_mc520encoder.h"
#elif MOTORGM37_ENABLE
#include "motor_gm37.h"
#endif

#endif /* MOTOR_MOTOR_H_ */
