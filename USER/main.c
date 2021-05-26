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
char receive_data[60];//接收到的字符
char state;				//电机正反转标志位0正转，1反转
char bsign;				//获得输入转动角度符号 bsign=0 正 bsign=1 负
int speed, angle;	//旋转速度，角度
int r, data = 0;  		//用于数据转换
int type;					//转换后的数据
extern u16 len;   //接收到的数据长度
extern u16 USART_RX_STA;


void receive_speed_and_angle()
{
    while(1)
    {
        if(USART_RX_STA & 0x8000)
        {
            len = USART_RX_STA & 0x3fff; //得到此次接收到的数据长度
            printf("\n The data is: \n\n");
            r = len;
            type = 0;
						bsign = 0;
					
            for(t = 0; t < len; t++)
            {
                USART_SendData(USART1, USART_RX_BUF[t]);//向串口1发送数据

                while(USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET); //等待发送结束

                //拷贝数据,将字符转换为十进制数
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
            //printf("\n\n");//插入换行
            USART_RX_STA = 0;
            break;
        }
    }
}


/************************************************
 ALIENTEK精英STM32开发板实验8
 定时器中断实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
	int cnt = 10;
    //int cnt = 0;
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
    LED_Init();			     //LED端口初始化
    pwm_init();

    GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	//delay_ms(30*1000);
	printf("输入测试次数，默认cnt=%d \n\n", cnt);
	receive_speed_and_angle();
	cnt=type;
    //Pulse_output(1000, 8000);//1KHZ，8000个脉冲
	
	// 测试循环
	while(cnt--){
		if(cnt%2)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_6);//电机正转
			Pulse_output(500, 500);//500HZ，500个脉冲
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_6);//电机反转
			Pulse_output(500, 500);//500HZ，500个脉冲
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

    // 延时5秒 供打开串口调试工具
    delay_ms(5000);
    printf("\r\n 输入速度为填充的ARR数值（频率f=10^6/speed）\r\n");
    printf("\r\n 输入角度为发出脉冲个数 \r\n");
    printf("\r\n 系统时钟频率为72MH，2分频 \r\n");
    printf("\r\n 请选择正反转，正转输入0，反转输入1 （以新行作为结束标志）\r\n");

//pwm_set(500, 10000);
    while (1)
    {
Initial_state:
			/*
        printf("\r\n 请选择正反转，正转输入0，反转输入1 （以新行作为结束标志）\r\n");
        receive_speed_and_angle();
        state = type;//将接收到的数据给type

        if (state == 0)//电机正转
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_6);//电机正转
            printf("\r\n 电机为正转模式，请输入旋转速度（rad/s），输入0返回初始模式 \r\n");
        }
        //---------------------反转模式------------------------------//
        else if (state == 1)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_6);//电机反转
            printf("\r\n 电机为反转模式，请输入旋转速度（rad/s），输入0返回初始模式 \r\n");

        }
			*/
				printf("\r\n 请输入旋转速度 \r\n");

        //---------------------配置速度参数------------------------------//
        receive_speed_and_angle();
        speed = type;//将接收到的数据给speed

        if (speed <= 0)
        {
            goto Initial_state;
        }//如果是0则返回初始模式
        else
        {
            printf("\r\n 电机旋转速度为%d rad/s，请输入旋转角度（+ --> 正转，- --> 反转），\
							输入0返回初始模式 \r\n", speed);
            //---------------------配置角度参数------------------------------//
            receive_speed_and_angle();
            angle = type;//将接收到的数据给type
        }

        while (1)
        {
					printf("\r\n 请输入旋转角度（+ --> 正转，- --> 反转），\
							输入0返回初始模式 \r\n");
					receive_speed_and_angle();
					 angle = type;//将接收到的数据给type
            if (angle > 0)//接收到的数据不是0
            {
							GPIO_SetBits(GPIOB, GPIO_Pin_6);//电机正转
                //通过GPIO延时输出脉冲 效率低 不精确
                //pwm_set(speed, angle);
                GPIO_ResetBits(GPIOB, GPIO_Pin_5); //点亮LED0
                Pulse_output(speed, angle);
                printf("\r\n 电机保护，请等2秒 \r\n");
                delay_ms(2000);//电机保护
                printf("\r\n 电机已旋转%d °，请输入下一次旋转角度，输入0返回初始模式； \r\n", angle);
                angle = 0;
                //receive_speed_and_angle();
                //angle = type;
            }
            else if (angle == 0)
            {
                goto Initial_state;//返回初始状态
            }
            else
            {
							GPIO_ResetBits(GPIOB, GPIO_Pin_6);//电机反转
							angle=-angle;
							Pulse_output(speed, angle);
                printf("\r\n 电机保护，请等2秒 \r\n");
                delay_ms(2000);//电机保护
                printf("\r\n 电机已旋转%d °，请输入下一次旋转角度，输入0返回初始模式； \r\n", angle);
                angle = 0;
                //receive_speed_and_angle();
                //angle = type;
//                printf("\r\n 角度错误，已返回初始模式 \r\n");
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


