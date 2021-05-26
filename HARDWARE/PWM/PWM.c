#include "PWM.h"
#include "delay.h"
#include <stdio.h>

void pwm_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	// PB5 PUL
	// PB6 DIR
	// PB7 ENA
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void pwm_set(u32 PulseFrequency, u32 pulse){
	u32 cnt=0;
	while(cnt<pulse){
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);
		delay_us(PulseFrequency);	
		GPIO_SetBits(GPIOB, GPIO_Pin_5);
		delay_us(PulseFrequency);	
		++cnt;
	}
}


/***TIM1 MASTER, TIM2 SLAVE***/
void TIM1_config(u32 Cycle)
{
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO���ã������ṹ��
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//��ʱ�����ýṹ��
    TIM_OCInitTypeDef  TIM_OCInitStructure; //pwm����Ӧ���ýṹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1 , ENABLE); //����ʱ��
 
		// TIM1_CH 1 2 3 4 -- PA8 PA9 PA10 PA11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //����ʱ�ӷָTDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //�߼���ʱ�� �ظ�������һ��Ҫ=0
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //װ��                        
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //ѡ��ʱ��ģʽ��TIM�����ȵ���ģʽ1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //���ô�װ�벶��Ĵ���������ֵ                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //�������       
 
 // ����ֻ�ܽ�PA11 PA8���в�֪��Ϊʲô
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);     //װ��ͨ��1,PA8 
    //TIM_OC2Init(TIM1, &TIM_OCInitStructure);     //װ��ͨ��2,PA9��PA10������ռ��
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);     //װ��ͨ��4,PA11
 
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    //���û������� TIMx ��/��ģʽ
    //TIMx�� x ������ 2�� 3 ���� 4����ѡ�� TIM ����
    //TIM_MasterSlaveMode����ʱ����/��ģʽ��TIM ��/��ģʽʹ��
    
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    //ѡ�� TIMx �������ģʽ
    //TIMx�� x ������ 2�� 3 ���� 4����ѡ�� TIM ����
    //TIM_TRGOSource���������ģʽ
    //TIM_TRGOSource_Update��ʹ�ø����¼���Ϊ���������TRGO��
    
 
//    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ�ܻ���ʧ�� TIMx �� CCR3 �ϵ�Ԥװ�ؼĴ���
//    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ�ܻ���ʧ�� TIMx �� CCR3 �ϵ�Ԥװ�ؼĴ���
    
    TIM_ARRPreloadConfig(TIM1, ENABLE);  // ʹ�ܻ���ʧ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ��� 
    //������ֹ�ڶ�ʱ������ʱ��ARR�Ļ�������д����ֵ���Ա��ڸ����¼�����ʱ���븲����ǰ��ֵ
}
 
/***��ʱ��2��ģʽ***/
void TIM2_config(u32 PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //��Ӧ�ṹ������
    NVIC_InitTypeDef NVIC_InitStructure;  //NVIC ��Ӧ�ṹ������
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  
 
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);//ѡ�� TIMx ���봥��Դ��TIM �ڲ����� 0      
    TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_External1 );// ��ͬ TIM2->SMCR|=0x07 //���ô�ģʽ�Ĵ��� 
    //   TIM2->SMCR|=0x07;                                  //���ô�ģʽ�Ĵ���       
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); //
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}
 
//����趨����
void Pulse_output(u32 Cycle,u32 PulseNum)
{
    TIM2_config(PulseNum);//װ��
    TIM_Cmd(TIM2, ENABLE);//ʹ��
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//���TIMx ���жϴ�����λ
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //ʹ�ܻ���ʧ��ָ���� TIMx �ж�
    TIM1_config(Cycle); //װ��   
    TIM_Cmd(TIM1, ENABLE);//ʹ��
    TIM_CtrlPWMOutputs(TIM1, ENABLE);   //�߼���ʱ��һ��Ҫ���ϣ������ʹ��
}
  
//�жϴ�����
void TIM2_IRQHandler(void) 
{ 
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)     // TIM_IT_CC1
    { 
				printf("\r\n TIM2_IRQHandler \r\n");
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־λ 
        TIM_CtrlPWMOutputs(TIM1, DISABLE);  //�����ʹ��
        TIM_Cmd(TIM1, DISABLE); // �رն�ʱ�� 
        TIM_Cmd(TIM2, DISABLE); // �رն�ʱ�� 
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
				GPIO_SetBits(GPIOB, GPIO_Pin_5); // LED0Ϩ��
    } 
} 
