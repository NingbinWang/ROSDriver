#include "app.h"
#include "config.h"
#include "esp8266.h"
#include "w25x16.h"
#include "canwork.h"
#include <string.h>
#include "ssd1306_conf.h"
#include "motor.h"
#include "mpu6050conf.h"
#include "key.h"
#include "protocol.h"

#define MOTOR_OUT_DEAD_VAL       0       //死区值
#define MOTOR_OUT_MAX           100       //占空比正最大值
#define MOTOR_OUT_MIN         (-100)   //占空比负最大值

#define CAR_ANGLE_SET 0//目标角度
#define CAR_ANGLE_SPEED_SET 0//目标角速度
float g_fCarAngle;//小车倾角




void App_Balance_Init()
{
    MotorMC520APWMStart();
	MotorMC520BPWMStart();
	MotorAEncoderInit();
	MotorBEncoderInit();
}

void App_Balance_Task()
{
    MotorMC520ASetSpeed(1, 0);
	MotorMC520BSetSpeed(1, 0);
}


void App_IMU_Init()
{
	int i = 0;
	for(i = 0;i<10;i++)
	{
		if(MPU6050_Init(MPU6050_I2C_PORT) == 1)
			break;
	}
	//if(i == 10)

		
}


void App_IMU_Task()
{
	  MPU6050_t data;
	  MPU6050_Read_All(MPU6050_I2C_PORT,&data);
	  //int AX_Protocol_SendPacket(uint8_t *pbuf, uint8_t len, uint8_t num)
	  //AX_Protocol_SendPacket((uint8_t*)&data.KalmanAngleY,1,1);
	  printf("@%f %f %f %f %f %f %f %f\r\n",data.Ax,data.Ay,data.Az,data.Gx,data.Gy,data.Gz,data.KalmanAngleX,data.KalmanAngleY);
	  g_fCarAngle = data.KalmanAngleY;

}


void App_Show_Init()
{
#if SSD1306_ENABLE
	ssd1306_Init();
#endif
}

void App_Show_Task()
{
	#if SSD1306_ENABLE
	char showcount[256];
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0,0);
	sprintf(showcount,"speed %d %d",GetMotorAEncoder(),GetMotorBEncoder());
	ssd1306_WriteString(showcount,Font_7x10,White);
	ssd1306_UpdateScreen();
	#endif
}

void App_Control_Init()
{

}

void App_Control_Task()
{
	ButtonScan();
}


uint8_t aRxBuffer;
PROTOCOL_UART_T protocolbuf = {0};

void App_Data_Init()
{
	HAL_UART_Receive_IT(AXProtocolUart, (uint8_t *)&aRxBuffer, 1);
}


void App_Data_Task()
{

    if(protocolbuf.uart_cnt > 32){
    	int i = 0;
    	for(i = 0; i < 32;i++){
    		printf("%x",protocolbuf.RxBuffer[i]);
    	}
    }

}





void App_Init()
{
	App_IMU_Init();
	App_Balance_Init();
	App_Show_Init();
	App_Control_Init();
	App_Control_Init();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
     UNUSED(huart);
    /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
  */
    if(huart->Instance == AXProtocolUartBus){
    	if(protocolbuf.uart_cnt > RXBUFFERSIZE)
    	{
    		protocolbuf.uart_cnt = 0;
    		memset(protocolbuf.RxBuffer,0x00,sizeof(protocolbuf.RxBuffer));
    	}else{
    		protocolbuf.RxBuffer[protocolbuf.uart_cnt++] = aRxBuffer;   //接收数据转存
    	}

    }
	HAL_UART_Receive_IT(AXProtocolUart, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
}
