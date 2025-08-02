/*
 * filters.h
 *
 *  Created on: Aug 2, 2025
 *      Author: Administrator
 */

#ifndef _ALGO_FILTERS_H_
#define _ALGO_FILTERS_H_

// 定义一阶低通滤波器结构体
typedef struct {
    float alpha;           // 时间常数
    float previous_output; // 上一时刻的输出
} LOWPASSFILTER_T;
// 一阶低通滤波函数
float LowPassFliter(LOWPASSFILTER_T* filter, float input);

#endif /* ALGO_INCLUDE_FILTERS_H_ */
