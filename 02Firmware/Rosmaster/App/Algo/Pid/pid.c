#include <stdio.h>
#include <math.h>
#include <stdint.h>


float PID_PidCtl(int32_t bias, int32_t bias_diff, int32_t bias_integral,float kP,float kD,float kI,float pidscale)
{
    return kP*bias*pidscale + kD*bias_diff*pidscale + kI*bias_integral*pidscale;

}



float PID_PdCtl(float target_angle,float current_angle,float target_speed,float current_speed,float fP,float fD)    
{
    return (target_angle - current_angle) * fP + (target_speed - current_speed) * fD;//PD控制器
}




float PID_PiCtl(float delta,float integral,float fP,float fI)
{
	return delta * fP + integral * fI; //PI控制器，输出=误差*P+误差积分*I 
}



