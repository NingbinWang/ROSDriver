#include "app.h"
#include "config.h"
#include "esp8266.h"
#include "esp8266test.h"
#include "w25x16.h"
#include "canwork.h"
#include <string.h>
#include "ssd1306_conf.h"
#include "motor.h"

void App_IMU_Init()
{

}


void App_Init()
{
	MotorMC520APWMStart();
	MotorMC520ASetSpeed(1, 1000);
	MotorMC520BPWMStart();
	MotorMC520BSetSpeed(1, 1000);
#if SSD1306_ENABLE
	ssd1306_Init();
#endif
}

void App_Show()
{
#if SSD1306_ENABLE
	char showcount[256];
	ssd1306_SetCursor(0,0);
	sprintf(showcount,"speed %d %d",11,14);
	ssd1306_WriteString(showcount,Font_7x10,White);
	ssd1306_UpdateScreen();
#endif
}



void App_Func()
{
	App_Show();
}
