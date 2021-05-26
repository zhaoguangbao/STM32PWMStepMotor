#ifndef PWM_TEST_H
#define PWM_TEST_H
#include "sys.h"

// 利用IO延时输出PWM 用于测试接线正确性
void pwm_init(void);
void pwm_set(u32 PulseFrequency, u32 pulse);


void Pulse_output(u32 Cycle,u32 PulseNum);//
 
void TIM1_config(u32 Cycle);//1 master
void TIM2_config(u32 PulseNum);//2 slave
 
//void TIM3_config(u32 PulseNum);//3 master
//void TIM4_config(u32 Cycle);//4 slave
 
//void TIM3_config(u32 PulseNum);//3 master
//void TIM4_config(u32 Cycle);//4 slave
 
//void TIM1_config(u32 PulseNum);//1 master
//void TIM3_config(u32 Cycle);//3 slave

//中断处理函数
void TIM2_IRQHandler(void);

#endif
