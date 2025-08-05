#ifndef _CANWORKCONF_H_
#define _CANWORKCONF_H_

#define CANWORK_ENABLE 1
#if (CANWORK_ENABLE)
#include "can.h"
#define canworkhcan &hcan1
#if INC_FREERTOS_H
#define     CAN_Delay                             osDelay
#else
#define     CAN_Delay                             HAL_Delay
#endif

#endif

#endif
