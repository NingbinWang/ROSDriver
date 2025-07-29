/*
 * motor_mc520.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Administrator
 */

#ifndef MOTOR_MC520_H_
#define MOTOR_MC520_H_
#include <stdint.h>
#include "motorconf.h"
#if MOTORMC520_ENABLE
void MotorMC520ASetSpeed(int8_t _direction, int16_t _speed);
void MotorMC520BSetSpeed(int8_t _direction, int16_t _speed);
void MotorMC520APWMStart();
void MotorMC520BPWMStart();
#endif


#endif /* MOTOR_MOTOR_MC520_H_ */
