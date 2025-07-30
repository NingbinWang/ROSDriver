#include <stdio.h>
#include <math.h>
#include <stdint.h>
#define PID_SCALE  0.01f  //PID缩放系数
//PID参数
int16_t ax_motor_kp=300;
int16_t ax_motor_ki=0;
int16_t ax_motor_kd=0;
/**
  * @简  述   PID控制函数
  * @参  数   spd_target:编码器速度目标值 ,范围（±250）
  *           spd_current: 编码器速度当前值
  * @返回值  电机PWM速度
  */
int16_t PID_MotorVelocityPidCtl(int16_t spd_target, int16_t spd_current)
{
	static int16_t motor_pwm_out;
	static int32_t bias,bias_last,bias_integral = 0;

	bias = spd_target - spd_current;

	bias_integral += bias;

	motor_pwm_out += ax_motor_kp*bias*PID_SCALE + ax_motor_kd*(bias-bias_last)*PID_SCALE + ax_motor_ki*bias_integral*PID_SCALE;

	bias_last = bias;

	return motor_pwm_out;
}
