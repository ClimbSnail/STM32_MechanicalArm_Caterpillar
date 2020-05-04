#include "sys.h"
#include "delay.h"  
#include "led.h"
#include "timer.h"
#include "usart.h"
#include "steering.h"
#include "my_gpio.h"
//ALIENTEK ̽����STM32F407������ ʵ��9
//PWM���ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾
  
int main(void)
{  
	Stm32_Clock_Init(336,8,2,7);//����ʱ��,168Mhz 
	delay_init(168);			//��ʱ��ʼ�� 
	my_gpioinit();
	LED_Init();
	uart1_init(84,115200);	
  TIM2_Int_Init(5000-1,84-1); 
	TIM5_Int_Init(5000-1,84-1);
	LED1=0;
	LED1=1;
	SREET2=1500;
	SREET3=500;
	SREET4=800;
	SREET5=1000;
	SREET6=1500;
	steering_init();
//	TIM3_Int_Init(50-1,8400-1);
//	cool_init();
  while(1)
	{
		RECEIVE_JUGE();  //���ڽ����ж�
		if( setting_flag == 2 )
			ative_auto();
	}
}