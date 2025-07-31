/*
 * key.h
 *
 *  Created on: Jul 31, 2025
 *      Author: Administrator
 */

#ifndef _KEY_H_
#define _KEY_H_
#include <stdint.h>
#include <stdio.h>
#include "key_conf.h"
#if KEY_ENABLE
typedef struct{
   int key1;
   int key2;
}BUTTON_T;

void ButtonScan(void);
BUTTON_T GetButton_status(void);

#endif

#endif /* KEY_KEY_H_ */
