#ifndef __DRV_MOTOR_H__
#define __DRV_MOTOR_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <string.h>

void motor_configuration(void);  //��ʼ������

void motor_control_F(int n);     //��ת������Ϊ����ת�ٴ�С
void motor_control_Z(int n);	//��ת������Ϊ����ת�ٴ�С
void Motor_Ctrl_Off(void);		//�رյ������ֹ�����ʱ�����������
void Motor_Ctrl_Angle(int angle,int n);		//����ת���Ƕȣ�����һ���Ƕ�ֵ��������������ת��

#endif
