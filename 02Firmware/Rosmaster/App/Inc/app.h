#ifndef _APP_H_
#define _APP_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
void App_Init();
void App_Balance_Task();
void App_IMU_Task();
void App_Show_Task();
void App_Control_Task();
void App_Data_Task();

void SetAnglePid(float Kp,float Ki,float Kd);
float GetAngleKp();
float GetAngleKd();
void SetSpeedPid(float Kp,float Ki,float Kd);
float GetSpeedKp();
float GetSpeedKi();
void SetDebugLevel(uint8_t level);
uint8_t GetDebugLevel();



extern unsigned int debug_level;

#define APP_ERR(fmt, ...) do { \
	if (debug_level > 0) \
			printf("[App][INFO]: "fmt, ##__VA_ARGS__); \
} while (0)


#define APP_WAR(fmt, ...) do { \
		if (debug_level > 1) \
			printf("[App][WAR]: "fmt, ##__VA_ARGS__); \
} while (0)

#define APP_TR(fmt, ...) do { \
		if (debug_level > 2) \
			printf("[App][ERR]: "fmt, ##__VA_ARGS__); \
} while (0)


#endif

