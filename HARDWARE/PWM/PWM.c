#include "PWM.h"
#include "delay.h"
#include <stdio.h>

void pwm_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA,PORTE时钟

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
    GPIO_InitTypeDef GPIO_InitStructure; //GPIO设置，创建结构体
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;//定时器设置结构体
    TIM_OCInitTypeDef  TIM_OCInitStructure; //pwm波对应设置结构体
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1 , ENABLE); //开启时钟
 
		// TIM1_CH 1 2 3 4 -- PA8 PA9 PA10 PA11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    TIM_TimeBaseStructure.TIM_Period = Cycle-1;                                                   
    TIM_TimeBaseStructure.TIM_Prescaler =71;                    //设置用来作为TIMx时钟频率除数的预分频值                                                     
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //设置时钟分割：TDTS= Tck_tim            
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;            //高级定时器 重复计数，一定要=0
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);               //装载                        
 
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;          //选择定时器模式：TIM脉冲宽度调制模式1       
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = Cycle/2-1;                    //设置待装入捕获寄存器的脉冲值                                   
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;      //输出极性       
 
 // 接线只能接PA11 PA8不行不知道为什么
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);     //装载通道1,PA8 
    //TIM_OC2Init(TIM1, &TIM_OCInitStructure);     //装载通道2,PA9，PA10被串口占用
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);     //装载通道4,PA11
 
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);
    //设置或者重置 TIMx 主/从模式
    //TIMx： x 可以是 2， 3 或者 4，来选择 TIM 外设
    //TIM_MasterSlaveMode：定时器主/从模式，TIM 主/从模式使能
    
    TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);
    //选择 TIMx 触发输出模式
    //TIMx： x 可以是 2， 3 或者 4，来选择 TIM 外设
    //TIM_TRGOSource：触发输出模式
    //TIM_TRGOSource_Update：使用更新事件作为触发输出（TRGO）
    
 
//    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能或者失能 TIMx 在 CCR3 上的预装载寄存器
//    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能或者失能 TIMx 在 CCR3 上的预装载寄存器
    
    TIM_ARRPreloadConfig(TIM1, ENABLE);  // 使能或者失能 TIMx 在 ARR 上的预装载寄存器 
    //允许或禁止在定时器工作时向ARR的缓冲器中写入新值，以便在更新事件发生时载入覆盖以前的值
}
 
/***定时器2从模式***/
void TIM2_config(u32 PulseNum)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //对应结构体声明
    NVIC_InitTypeDef NVIC_InitStructure;  //NVIC 对应结构体声明
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
 
    TIM_TimeBaseStructure.TIM_Period = PulseNum-1;   
    TIM_TimeBaseStructure.TIM_Prescaler =0;    
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  
 
    TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);//选择 TIMx 输入触发源，TIM 内部触发 0      
    TIM_SelectSlaveMode(TIM2,TIM_SlaveMode_External1 );// 等同 TIM2->SMCR|=0x07 //设置从模式寄存器 
    //   TIM2->SMCR|=0x07;                                  //设置从模式寄存器       
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); //
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;        
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;     
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure);
}
 
//入口设定函数
void Pulse_output(u32 Cycle,u32 PulseNum)
{
    TIM2_config(PulseNum);//装载
    TIM_Cmd(TIM2, ENABLE);//使能
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除TIMx 的中断待处理位
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //使能或者失能指定的 TIMx 中断
    TIM1_config(Cycle); //装载   
    TIM_Cmd(TIM1, ENABLE);//使能
    TIM_CtrlPWMOutputs(TIM1, ENABLE);   //高级定时器一定要加上，主输出使能
}
  
//中断处理函数
void TIM2_IRQHandler(void) 
{ 
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)     // TIM_IT_CC1
    { 
				printf("\r\n TIM2_IRQHandler \r\n");
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位 
        TIM_CtrlPWMOutputs(TIM1, DISABLE);  //主输出使能
        TIM_Cmd(TIM1, DISABLE); // 关闭定时器 
        TIM_Cmd(TIM2, DISABLE); // 关闭定时器 
        TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); 
				GPIO_SetBits(GPIOB, GPIO_Pin_5); // LED0熄灭
    } 
} 
