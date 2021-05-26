#ifndef PWM_TEST_H
#define PWM_TEST_H
#include "sys.h"

// ����IO��ʱ���PWM ���ڲ��Խ�����ȷ��
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

//�жϴ�����
void TIM2_IRQHandler(void);

#endif
