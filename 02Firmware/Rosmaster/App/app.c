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
#include "pid.h"
#include "filters.h"
#include "power.h"
#include "sys_queue.h"
#include "sys_commandline.h"
#include "esp8266.h"
#include "wifiapp.h"
#include "led.h"

//#define APP_TR()
unsigned int debug_level = 0;

bool wifion = false;
MPU6050_t mpu6050data;
//Ayou b zuo
//编码器控制
int16_t gAMotorencoderPulse;	//左编码器绝对值
int16_t gBMotorencoderPulse;	//右编码器绝对值
int16_t gAMotorencoderdelta;	//左编码器相对变化值,代表实际速度
int16_t gBMotorencoderdelta;	//右编码器相对变化值,代表实际速度
long gAMotorPulseSigma;//左电机25ms内累计脉冲总和
long gBMotorPulseSigma;//右电机25ms内累计脉冲总和




//全局变量
int16_t gAMotorPwm;  //电机PWM速度
int16_t gBMotorPwm;  //电机PWM速度
int16_t gSpeedTarget = 0;//全局变量，速度目标值

//速度内循环PID参数
int16_t gVelocityMotorKp=300;
int16_t gVelocityMotorKi=0;
int16_t gVelocityMotorKd=0;
float gVelocityPidScale=0.01f;  //PID缩放系数

int16_t gPositionMotorKp=500;
int16_t gPositionMotorKi=0;
int16_t gPositionMotorKd=0;
float gPositionPidScale=0.01f;  //PID缩放系数

//角速度PID
float gAngleKp = 5.0;//角度环P参数
float gAngleKd = 0.2;//角度环D参数


#define MOTOR_OUT_DEAD_VAL       0       //死区值
#define MOTOR_OUT_MAX           100       //占空比正最大值
#define MOTOR_OUT_MIN         (-100)   //占空比负最大值



#define CAR_ANGLE_SET 0//目标角度
#define CAR_ANGLE_SPEED_SET 0//目标角速度


float gfCarAngle;//小车倾角
float gfAngleControlOut;
float gfGyroAngleSpeed;

void GetMotorPulse()
{
	//计算编码器速度
	gAMotorencoderPulse = GetMotorAEncoderCount()-30000;
	gBMotorencoderPulse = GetMotorBEncoderCount()-30000;
	//设置编码器初始中间值
	SetMotorMC520AEncoderCount(30000);
	SetMotorMC520BEncoderCount(30000);
	//累积值
	gAMotorPulseSigma += gAMotorencoderPulse;
	gBMotorPulseSigma += gBMotorencoderPulse;
}


/**
  * @简  述   PID控制函数 速度左内循环
  * @参  数   spd_target:编码器速度目标值 ,范围（±250）
  *           spd_current: 编码器速度当前值
  * @返回值   电机PWM速度
  */
void PID_MotorAVelocityPidCtl(int16_t spd_target)
{
	static int32_t Abias,Abias_last,Abias_integral = 0;
	Abias = spd_target - gAMotorencoderPulse;
	Abias_integral += Abias;
	gAMotorPwm = PID_PidCtl(Abias,Abias-Abias_last,Abias_integral,gVelocityMotorKp,gVelocityMotorKd,gVelocityMotorKi,gVelocityPidScale);
	Abias_last = Abias;
}
/**
  * @简  述   PID控制函数 速度右内循环
  * @参  数   spd_target:编码器速度目标值 ,范围（±250）
  *           spd_current: 编码器速度当前值
  * @返回值   电机PWM速度
  */
void PID_MotorBVelocityPidCtl(int16_t spd_target)
{
	static int32_t Bbias,Bbias_last,Bbias_integral = 0;
	Bbias = spd_target - gBMotorencoderPulse;
	Bbias_integral += Bbias;
	gBMotorPwm = PID_PidCtl(Bbias,Bbias-Bbias_last,Bbias_integral,gVelocityMotorKp,gVelocityMotorKd,gVelocityMotorKi,gVelocityPidScale);
	Bbias_last = Bbias;
}


void PID_MotorAPositionPidCtl(int16_t ptn_target)
{
	static int32_t Aptnbias,Aptnbias_last,Aptnbias_integral = 0;
	Aptnbias = ptn_target - gAMotorencoderPulse;
	Aptnbias_integral += Aptnbias;
	gAMotorPwm = PID_PidCtl(Aptnbias,Aptnbias-Aptnbias_last,Aptnbias_integral,gPositionMotorKp,gPositionMotorKd,gPositionMotorKi,gPositionPidScale);
	Aptnbias_last = Aptnbias;
}

void PID_MotorBPositionPidCtl(int16_t ptn_target)
{
	static int32_t Bptnbias,Bptnbias_last,Bptnbias_integral = 0;
	Bptnbias = ptn_target - gBMotorencoderPulse;
	Bptnbias_integral += Bptnbias;
	gBMotorPwm = PID_PidCtl(Bptnbias,Bptnbias-Bptnbias_last,Bptnbias_integral,gPositionMotorKp,gPositionMotorKd,gPositionMotorKi,gPositionPidScale);
	Bptnbias_last = Bptnbias;
}

/**
  * @简  述   PID控制函数 角度环控制函数
  * @参  数   spd_target:编码器速度目标值 ,范围（±250）
  *           spd_current: 编码器速度当前值
  * @返回值   电机PWM速度
  */
void AngleControl(void)     //角度环控制函数
{
	gfAngleControlOut = PID_PdCtl(CAR_ANGLE_SET,gfCarAngle,CAR_ANGLE_SPEED_SET,gfGyroAngleSpeed,gAngleKp,gAngleKd);
}


float gfCarSpeed;//小车实际速度
LOWPASSFILTER_T carspeedlowpassfilter;
float gfCarSpeedLowpassAlpha;//低通滤波常数
float gfCarPosition;//小车路程
#define CAR_SPEED_SET 0//小车目标速度
float gfSpeedControlOut = 0;
//速度外循环
float gSpeedKp = 0.6;
float gSpeedKi = 0.03;

float PID_PiCtl(float delta,float integral,float fP,float fI);
void SpeedControl(void)//速度外环控制函数
{
    float fDelta;//临时变量，用于存储误差

    gfCarSpeed = (gAMotorPulseSigma + gBMotorPulseSigma ) / 2;//左轮和右轮的速度平均值等于小车速度
    gAMotorPulseSigma = 0;      //全局变量，注意及时清零
    gBMotorPulseSigma = 0;      //全局变量，注意及时清零
    carspeedlowpassfilter.alpha =gfCarSpeedLowpassAlpha;
    gfCarSpeed = LowPassFliter(&carspeedlowpassfilter,gfCarSpeed);//低通滤波，使速度更平滑
    fDelta = CAR_SPEED_SET - gfCarSpeed;//误差=目标速度-实际速度
    gfCarPosition +=fDelta;//对速度误差进行积分
    gfSpeedControlOut = PID_PiCtl(fDelta,gfCarPosition,gSpeedKp,gSpeedKi);
}


void SetMotorVoltage()//设置电机电压和方向
{
	gAMotorPwm = gfAngleControlOut-gfSpeedControlOut;
	gBMotorPwm = gfAngleControlOut-gfSpeedControlOut;
	if(gAMotorPwm > 2000)
		gAMotorPwm = 2000;
	if(gBMotorPwm > 2000)
		gBMotorPwm = 2000;

    if(gAMotorPwm > 0){
    	MotorMC520ASetSpeed(0,gAMotorPwm);
    }else if(gAMotorPwm < 0){
    	MotorMC520ASetSpeed(1,gAMotorPwm);
    }else{
    	gAMotorPwm = 0;
    	MotorMC520A_Stop();
    }
    if(gBMotorPwm > 0){
    	MotorMC520BSetSpeed(0,gBMotorPwm);
    }else if(gBMotorPwm < 0){
    	MotorMC520BSetSpeed(1,gBMotorPwm);
    }else{
    	gBMotorPwm = 0;
    	MotorMC520B_Stop();
    }
}


void SetAnglePid(float Kp,float Ki,float Kd)
{
	gAngleKp = Kp;
	gAngleKd = Kd;
}

float GetAngleKp()
{
	return gAngleKp ;
}

float GetAngleKd()
{
	return gAngleKd ;
}

float GetSpeedKp(void)
{
	return gSpeedKp;
}

float GetSpeedKi(void)
{
	return gSpeedKi;
}



void SetSpeedPid(float Kp,float Ki,float Kd)
{
	gSpeedKp = Kp;
	gSpeedKi = Ki;
}




void SetDebugLevel(uint8_t level)
{
   debug_level = level;
}

uint8_t GetDebugLevel()
{
   return debug_level;
}



void App_Balance_Init()
{
    MotorMC520APWMStart();
	MotorMC520BPWMStart();
	MotorAEncoderInit();
	MotorBEncoderInit();
	SetMotorMC520AEncoderCount(30000);
	SetMotorMC520BEncoderCount(30000);

}
int gSpeedControlCount = 0;
void App_Balance_Task()
{
	GetMotorPulse();
	//PID控制 速度内循环
	//PID_MotorAVelocityPidCtl(gSpeedTarget);
	//PID_MotorBVelocityPidCtl(gSpeedTarget);
	AngleControl();//角度控制
	gSpeedControlCount ++;
	if(gSpeedControlCount >= 5){
		SpeedControl();
	}
    //电机执行动作
	SetMotorVoltage();
	//GREEN_LEDSET();
}


void App_IMU_Init()
{
	int i = 0;
	for(i = 0;i<10;i++)
	{
		if(MPU6050_Init(MPU6050_I2C_PORT) == 1)
			break;
	}
}


void App_IMU_Task()
{

	  MPU6050_Read_All(MPU6050_I2C_PORT,&mpu6050data);
	  //data.KalmanAngleX为roll
	  //data.KalmanAngleY为ptich
	  APP_WAR("@%f %f %f %f %f %f %f %f\r\n",mpu6050data.Ax,mpu6050data.Ay,mpu6050data.Az,mpu6050data.Gx,mpu6050data.Gy,mpu6050data.Gz,mpu6050data.KalmanAngleX,mpu6050data.KalmanAngleY);
	  gfCarAngle = mpu6050data.KalmanAngleY;
	  gfGyroAngleSpeed = mpu6050data.Gx;

}


void App_Show_Init()
{
#if SSD1306_ENABLE
	ssd1306_Init();
#endif
#if SYS_COMMANDLINE_ENABLE
	CLI_INIT();
#endif
#if POWER_ADC_ENABLE
	Power_ADCInit();
#endif

}
uint8_t OLED = 0;
typedef enum{
	OLED_Main = 0,
	OLED_PID = 1,
	OLED_MAX,

}OLED_MENU_E;
void App_Show_Task()
{
#if SSD1306_ENABLE
	char showcount[256];
	ssd1306_Fill(Black);
	switch (OLED)
	{
		case OLED_Main:
			//--------------------
			ssd1306_SetCursor(0,0);
			sprintf(showcount,"SP: %d %d",gAMotorencoderPulse,gBMotorencoderPulse);
			ssd1306_WriteString(showcount,Font_7x10,White);
			//--------------------
			ssd1306_SetCursor(0,Font_7x10.FontHeight);
			sprintf(showcount,"CA:%.2f ",gfCarAngle);
			ssd1306_WriteString(showcount,Font_7x10,White);
			//--------------------
			ssd1306_SetCursor(0,2*Font_7x10.FontHeight);
			sprintf(showcount,"PWM:%d %d",gAMotorPwm,gBMotorPwm);
			ssd1306_WriteString(showcount,Font_7x10,White);
			//--------------------
			break;
		case OLED_PID:
			//--------------------
			ssd1306_SetCursor(0,0);
			sprintf(showcount,"APID: p:%.4f d:%.4f",gAngleKp,gAngleKd);
			ssd1306_WriteString(showcount,Font_7x10,White);
			//--------------------
			ssd1306_SetCursor(0,Font_7x10.FontHeight);
			sprintf(showcount,"SPID: p:%.4f i:%.4f",gSpeedKp,gSpeedKi);
			ssd1306_WriteString(showcount,Font_7x10,White);
			//--------------------
#if POWER_ADC_ENABLE
			ssd1306_SetCursor(0,2*Font_7x10.FontHeight);
			sprintf(showcount,"PWR:%d",Power_get_value());
			ssd1306_WriteString(showcount,Font_7x10,White);
#endif
			break;
	}
	ssd1306_UpdateScreen();
#endif
#if SYS_COMMANDLINE_ENABLE
	CLI_RUN();
#endif
	ButtonScan();

}

void App_Control_Init()
{
	wifion = Wifi_AppInit("xxxxx","xxxxx");//请修改成自己的WIFI的ssid与密码
	if(wifion == false)
		APP_ERR("wifi can't connect\r\n");
}

void App_Control_Task()
{
	
	BUTTON_T btn;
	GetButton_status(&btn);
	if(btn.key1 == 1)
	{
	   OLED++;
	   if(OLED >=OLED_MAX)
	   	OLED = 0;
	}
}



uint8_t aRxBuffer;
PROTOCOL_UART_T protocolbuf = {0};
uint8_t connect_count = 0;
void App_Data_Init()
{
#if SYS_COMMANDLINE_ENABLE
	HAL_UART_Receive_IT(PRINTFUart, (uint8_t *)&aRxBuffer, 1);	 //再开启接收中断
#endif
#if AX_PROTOCOL_ENABEL
	HAL_UART_Receive_IT(AXProtocolUart, (uint8_t *)&aRxBuffer, 1);
#endif
}


void App_Data_Task()
{
#if ANOT_PROTOCOL_ENABLE
     int16_t tmpdata[6] = {0};
     uint8_t data[256] = {0};
     tmpdata[0] = mpu6050data.Accel_X_RAW;
     tmpdata[1] = mpu6050data.Accel_Y_RAW;
     tmpdata[2] = mpu6050data.Accel_Z_RAW;
     tmpdata[3] = mpu6050data.Gyro_X_RAW;
     tmpdata[4] = mpu6050data.Gyro_Y_RAW;
     tmpdata[5] = mpu6050data.Gyro_Z_RAW;
     memcpy((void*)data,(void*)tmpdata,sizeof(uint8_t)*6*2);
     data[sizeof(uint8_t)*6*2] = 0;
     ANO_Protocol_SendPacket(data,13,ANTO_SENSOR);
#endif
}





void App_Init()
{
	App_IMU_Init();
	App_Balance_Init();
	App_Show_Init();
	App_Control_Init();
	App_Data_Init();
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    /* Prevent unused argument(s) compilation warning */
      UNUSED(huart);
    /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
    */
#if SYS_COMMANDLINE_ENABLE
    if(huart->Instance == PRINTFUartBus){
	    QUEUE_IN(cli_rx_buff, aRxBuffer);
    	//HAL_UART_Transmit(&huart1, (uint8_t *)aRxBuffer, 1, 0xffff);
    	// HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, 1);
		HAL_UART_Receive_IT(PRINTFUart, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
	}
#endif
#if AX_PROTOCOL_ENABEL
    if(huart->Instance == AXProtocolUartBus){
    	if(protocolbuf.uart_cnt > RXBUFFERSIZE)
    	{
    		protocolbuf.uart_cnt = 0;
    		memset(protocolbuf.RxBuffer,0x00,sizeof(protocolbuf.RxBuffer));
    	}else{
    		protocolbuf.RxBuffer[protocolbuf.uart_cnt++] = aRxBuffer;   //接收数据转存
    	}
		HAL_UART_Receive_IT(AXProtocolUart, (uint8_t *)&aRxBuffer, 1);   //再开启接收中断
    }
#endif
#if ESP8266_ENBALE
    if(huart->Instance == ESP8266_USART_BUS){
    	esp8266_RxCallBack();
    }
#endif
	
}
