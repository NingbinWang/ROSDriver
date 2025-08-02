/*
 * power_adc.c
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */
#include "power_conf.h"
#if POWER_ADC_ENABLE

void Power_ADCInit()
{
  HAL_ADC_Start(POWER_ADC);
}


uint16_t Power_get_value()
{
    if(HAL_OK == HAL_ADC_PollForConversion(POWER_ADC, 10))
		return HAL_ADC_GetValue(POWER_ADC);
	return 0;
}

#endif

