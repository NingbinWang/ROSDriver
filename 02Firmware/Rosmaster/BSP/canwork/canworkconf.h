#ifndef _CANWORKCONF_H_
#define _CANWORKCONF_H_

#define CANWORK_ENABLE 0
#if (CANWORK_ENABLE)
#include "can.h"
#define canworkhcan &hcan1
#endif

#endif
