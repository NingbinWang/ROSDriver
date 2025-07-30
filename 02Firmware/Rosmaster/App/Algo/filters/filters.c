
#include <stdio.h>
#include <math.h>

//互补滤波
// a = tau / (tau + dt)   加权系数
// acc = 加速度传感器数据 
// gyro = 陀螺仪数据 
// dt = 运行周期  
float ComplementaryFliter(float acc, float gyro, float dt,float a) 
{
	float angle;
    a = 0.98;  
    angle = a * (angle + gyro * dt) + (1 - a) * (acc);  
    return angle;  
}

