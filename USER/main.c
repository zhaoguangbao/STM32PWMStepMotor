#include "math.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "PWM.h"
#include "motor.h"
#include "usart.h"

u16 t;
u16 len;
u16 times = 0;
char receive_data[60];//���յ����ַ�
char state;				//�������ת��־λ0��ת��1��ת
char bsign;				//�������ת���Ƕȷ��� bsign=0 �� bsign=1 ��
int speed, angle;	//��ת�ٶȣ��Ƕ�
int r, data = 0;  		//��������ת��
int type;					//ת���������
extern u16 len;   //���յ������ݳ���
extern u16 USART_RX_STA;


void receive_speed_and_angle()
{
    while(1)
    {
        if(USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3fff; //�õ��˴ν��յ������ݳ���
            printf("\n The data is: \n\n");
            r = len;
            type = 0;
						bsign = 0;
					
            for(t = 0; t < len; t++)
            {
                USART_SendData(USART1, USART_RX_BUF[t]);//�򴮿�1��������

                while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); //�ȴ����ͽ���

                //��������,���ַ�ת��Ϊʮ������
                receive_data[t] = USART_RX_BUF[t];
								if(receive_data[t] == '-'){
										bsign = 1;
										data = 0;
								}else{
										data = (int)receive_data[t] - 48;
								}
                
                r = r - 1;
                type = type + data * (pow(10, r));
            }
						printf("\n\n type=%d, bsign = %d \n\n", type, bsign);
						if(bsign)
							type = -type;
            //printf("\n\n");//���뻻��
            USART_RX_STA = 0;
            break;
        }
    }
}


/************************************************
 ALIENTEK��ӢSTM32������ʵ��8
 ��ʱ���ж�ʵ��
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

int main(void)
{
	int cnt = 10;
    //int cnt = 0;
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
    LED_Init();			     //LED�˿ڳ�ʼ��
    pwm_init();

    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	//delay_ms(30*1000);
	printf("������Դ�����Ĭ��cnt=%d \n\n", cnt);
	receive_speed_and_angle();
	cnt=type;
    //Pulse_output(1000, 8000);//1KHZ��8000������
	
	// ����ѭ��
	while(cnt--){
		if(cnt%2)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);//�����ת
			Pulse_output(500, 500);//500HZ��500������
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//�����ת
			Pulse_output(500, 500);//500HZ��500������
		}
			
		delay_ms(5000);
	}





    /*
    	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    	delay_ms(1000);
    	GPIO_SetBits(GPIOB, GPIO_Pin_5);*/
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    /*
    motor_configuration();
    Motor_Ctrl_Angle(360,2100);
     //Motor_Ctrl_Angle(360,210000);
    Motor_Ctrl_Off();*/

    // ��ʱ5�� ���򿪴��ڵ��Թ���
    delay_ms(5000);
    printf("\r\n �����ٶ�Ϊ����ARR��ֵ��Ƶ��f=10^6/speed��\r\n");
    printf("\r\n ����Ƕ�Ϊ����������� \r\n");
    printf("\r\n ϵͳʱ��Ƶ��Ϊ72MH��2��Ƶ \r\n");
    printf("\r\n ��ѡ������ת����ת����0����ת����1 ����������Ϊ������־��\r\n");

//pwm_set(500, 10000);
    while (1)
    {
Initial_state:
			/*
        printf("\r\n ��ѡ������ת����ת����0����ת����1 ����������Ϊ������־��\r\n");
        receive_speed_and_angle();
        state = type;//�����յ������ݸ�type

        if (state == 0)//�����ת
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_6);//�����ת
            printf("\r\n ���Ϊ��תģʽ����������ת�ٶȣ�rad/s��������0���س�ʼģʽ \r\n");
        }
        //---------------------��תģʽ------------------------------//
        else if (state == 1)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);//�����ת
            printf("\r\n ���Ϊ��תģʽ����������ת�ٶȣ�rad/s��������0���س�ʼģʽ \r\n");

        }
			*/
				printf("\r\n ��������ת�ٶ� \r\n");

        //---------------------�����ٶȲ���------------------------------//
        receive_speed_and_angle();
        speed = type;//�����յ������ݸ�speed

        if (speed <= 0)
        {
            goto Initial_state;
        }//�����0�򷵻س�ʼģʽ
        else
        {
            printf("\r\n �����ת�ٶ�Ϊ%d rad/s����������ת�Ƕȣ�+ --> ��ת��- --> ��ת����\
							����0���س�ʼģʽ \r\n", speed);
            //---------------------���ýǶȲ���------------------------------//
            receive_speed_and_angle();
            angle = type;//�����յ������ݸ�type
        }

        while (1)
        {
					printf("\r\n ��������ת�Ƕȣ�+ --> ��ת��- --> ��ת����\
							����0���س�ʼģʽ \r\n");
					receive_speed_and_angle();
					 angle = type;//�����յ������ݸ�type
            if (angle > 0)//���յ������ݲ���0
            {
							GPIO_SetBits(GPIOB, GPIO_Pin_6);//�����ת
                //ͨ��GPIO��ʱ������� Ч�ʵ� ����ȷ
                //pwm_set(speed, angle);
                GPIO_ResetBits(GPIOB, GPIO_Pin_5); //����LED0
                Pulse_output(speed, angle);
                printf("\r\n ������������2�� \r\n");
                delay_ms(2000);//�������
                printf("\r\n �������ת%d �㣬��������һ����ת�Ƕȣ�����0���س�ʼģʽ�� \r\n", angle);
                angle = 0;
                //receive_speed_and_angle();
                //angle = type;
            }
            else if (angle == 0)
            {
                goto Initial_state;//���س�ʼ״̬
            }
            else
            {
							GPIO_ResetBits(GPIOB, GPIO_Pin_6);//�����ת
							angle=-angle;
							Pulse_output(speed, angle);
                printf("\r\n ������������2�� \r\n");
                delay_ms(2000);//�������
                printf("\r\n �������ת%d �㣬��������һ����ת�Ƕȣ�����0���س�ʼģʽ�� \r\n", angle);
                angle = 0;
                //receive_speed_and_angle();
                //angle = type;
//                printf("\r\n �Ƕȴ����ѷ��س�ʼģʽ \r\n");
//                goto Initial_state;
            }
        }
    }




    //pwm_set(500, 10000);
    /*while(1)
    {
    	//LED0=!LED0;
    	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
    	delay_ms(500);
    	GPIO_SetBits(GPIOB, GPIO_Pin_5);
    		delay_ms(500);
    	++cnt;
    	if(cnt>=10000){
    		GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    	}
    }*/
}


