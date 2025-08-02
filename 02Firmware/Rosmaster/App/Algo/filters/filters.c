
#include <stdio.h>
#include <math.h>
#include "filters.h"
//互补滤波
// a = tau / (tau + dt)   加权系数
// acc = 加速度传感器数据 
// gyro = 陀螺仪数据 
// dt = 运行周期  
float ComplementaryFliter(float acc, float gyro, float dt,float a) 
{
	float angle;
    angle = a * (angle + gyro * dt) + (1 - a) * (acc);  
    return angle;  
}

/*
* 低通滤波（Low Pass Filter）用于从一个信号中去除高于某个频率的成分。它的基本原理是，信号中高于某个频率的成分在信号传输或接收过程中会发生衰减，而低于该频率的成分则不受影响。因此，通过将信号通过一个低通滤波器，可以去除高频噪声，保留信号中的低频成分。
*一阶低通滤波器是低通滤波的一阶离散形式，用于滤除输入信号中的高频分量，只保留低频分量。它通过减弱高频部分的幅度，从而实现对信号的平滑处理。一阶低通滤波器的基本原理涉及限制信号的变化速率，对快速变化的信号进行衰减，而对缓慢变化的信号保留。
Y[k]=AX[k]+(1−A)Y[k−1]
在这个方程中，A越小，时间常数越大，低通滤波器的截止频率就越低，对高频部分的抑制效果就越强。
一阶低通滤波器常用于需要平滑信号或去除高频噪声的应用场景。它们在信号处理、通信系统、控制系统等领域都有广泛的应用
*/

// 一阶低通滤波函数
float LowPassFliter(LOWPASSFILTER_T* filter, float input) {
    // 计算输出
    float output = (1.0 - filter->alpha) * filter->previous_output + filter->alpha * input;
    // 更新上一次的输出
    filter->previous_output = output;
    return output;
}
