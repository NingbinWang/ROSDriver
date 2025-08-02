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
    	MotorMC520ASetSpeed(0,-gAMotorPwm);
    }else if(gAMotorPwm < 0){
    	MotorMC520ASetSpeed(1,-gAMotorPwm);
    }else{
    	gAMotorPwm = 0;
    	MotorMC520A_Stop();
    }
    if(gBMotorPwm > 0){
    	MotorMC520BSetSpeed(0,-gBMotorPwm);
    }else if(gBMotorPwm < 0){
    	MotorMC520BSetSpeed(1,-gBMotorPwm);
    }else{
    	gBMotorPwm = 0;
    	MotorMC520B_Stop();
    }
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
	  MPU6050_t data;
	  MPU6050_Read_All(MPU6050_I2C_PORT,&data);
	  //data.KalmanAngleX为roll
	  //data.KalmanAngleY为ptich
	  //printf("@%f %f %f %f %f %f %f %f\r\n",data.Ax,data.Ay,data.Az,data.Gx,data.Gy,data.Gz,data.KalmanAngleX,data.KalmanAngleY);
	  gfCarAngle = data.KalmanAngleY;
	  gfGyroAngleSpeed = data.Gx;

}


void App_Show_Init()
{
#if SSD1306_ENABLE
	ssd1306_Init();
#endif
#if SYS_COMMANDLINE_ENABLE
		CLI_INIT(CLI_BAUDRATE);
#endif


}

void App_Show_Task()
{
#if SSD1306_ENABLE
	char showcount[256];
	ssd1306_Fill(Black);
	ssd1306_SetCursor(0,0);
	sprintf(showcount,"speed %d %d ",gAMotorencoderPulse,gBMotorencoderPulse);
	ssd1306_WriteString(showcount,Font_7x10,White);
	ssd1306_SetCursor(0,Font_7x10.FontHeight);
	sprintf(showcount,"CarAngle:%f %f %f ",gfCarAngle,gfSpeedControlOut,gfSpeedControlOut);
	ssd1306_UpdateScreen();
#endif
#if SYS_COMMANDLINE_ENABLE
		CLI_RUN();
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
