#ifndef _PID_H_
#define _PID_H_


float PID_PidCtl(int32_t bias, int32_t bias_diff, int32_t bias_integral,float kP,float kD,float kI,float pidscale);
float PID_PdCtl(float target_angle,float current_angle,float target_speed,float current_speed,float fP,float fD);
float PID_PiCtl(float delta,float integral,float fP,float fI);



#endif
