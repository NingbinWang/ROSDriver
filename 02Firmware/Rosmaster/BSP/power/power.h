#ifndef _POWER_H_
#define _POWER_H_
#include "power_conf.h"
#if POWER_ADC_ENABLE
void Power_ADCInit();
uint16_t Power_get_value();
#endif


#endif
