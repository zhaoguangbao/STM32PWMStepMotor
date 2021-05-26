#ifndef __DRV_MOTOR_H__
#define __DRV_MOTOR_H__

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include <string.h>

void motor_configuration(void);  //初始化引脚

void motor_control_F(int n);     //反转，参数为控制转速大小
void motor_control_Z(int n);	//正转，参数为控制转速大小
void Motor_Ctrl_Off(void);		//关闭电机，防止因接入时间过长而发热
void Motor_Ctrl_Angle(int angle,int n);		//控制转动角度，参数一：角度值，参数二：控制转速

#endif
