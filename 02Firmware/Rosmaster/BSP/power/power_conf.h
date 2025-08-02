/*
 * power_conf.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _POWER_CONF_H_
#define	_POWER_CONF_H_

#define POWER_ADC_ENABLE 1
#include "adc.h"
#if POWER_ADC_ENABLE

#define POWER_ADC &hadc2

#endif

#endif /* POWER_POWER_CONF_H_ */
