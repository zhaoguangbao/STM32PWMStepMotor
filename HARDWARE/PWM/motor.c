#include "motor.h"
#include "delay.h"


#define orange_H GPIO_SetBits(GPIOB, GPIO_Pin_15);
#define orange_L GPIO_ResetBits(GPIOB, GPIO_Pin_15);

#define yellow_H GPIO_SetBits(GPIOB, GPIO_Pin_14);
#define yellow_L GPIO_ResetBits(GPIOB, GPIO_Pin_14);

#define pink_H   GPIO_SetBits(GPIOB, GPIO_Pin_13);
#define pink_L	 GPIO_ResetBits(GPIOB, GPIO_Pin_13);

#define blue_H   GPIO_SetBits(GPIOB, GPIO_Pin_12);
#define blue_L   GPIO_ResetBits(GPIOB, GPIO_Pin_12);


void motor_configuration(void)//PB12,PB13,PB14,PB15	
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
		delay_init();
}

void motor_control_F(int n)
{
		orange_H;
		delay_us(n);
		blue_L;
		delay_us(n);
		yellow_H;
		delay_us(n);
		orange_L;
		delay_us(n);
		pink_H;
		delay_us(n);
		yellow_L;
		delay_us(n);
		blue_H;
		delay_us(n);
		pink_L;
		delay_us(n);
}

void motor_control_Z(int n)
{
		blue_H;
		delay_us(n);
		orange_L;
		delay_us(n);
		pink_H;
		delay_us(n);
		blue_L;
		delay_us(n);
		yellow_H;
		delay_us(n);
		pink_L;
		delay_us(n);
		orange_H;
		delay_us(n);
		yellow_L; 
		delay_us(n);
	
}

void Motor_Ctrl_Off(void){
	GPIO_ResetBits(GPIOB, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
}

void Motor_Ctrl_Angle(int angle,int n){
	u16 j;
	for(j=0;j<64*angle/45;j++) 
	{
		motor_control_F(n);
	}
}

