/*
 * WifiApp.c
 *
 *  Created on: Aug 2, 2025
 *      Author: Administrator
 */

#include "esp8266.h"
#include "wifiapp.h"
bool Wifi_AppInit(char *SSID,char *Password)
{
	bool connected = false;
#if ESP8266_ENBALE
	bool isget = false;
	int i = 0;
	esp8266_Init();
	//esp8266_Restart();
	esp8266_Echo(0);
	esp8266_GetVersion();
	esp8266_UartCheck();
	esp8266_SetMode(WifiMode_Station);
	do{
		connected = esp8266_Station_ConnectToAp(SSID,Password,NULL);
		if(connected == true)
			break;
		i++;
	}while (i < 100);


	isget = esp8266_TcpIp_StartUdpConnection(0,"192.168.31.17",1000,500);
	if(isget == true){
		if(esp8266_GetMyIp())
			printf("ip:%s\r\n",Wifi.MyIP);
	}
#endif

	return connected;
}
