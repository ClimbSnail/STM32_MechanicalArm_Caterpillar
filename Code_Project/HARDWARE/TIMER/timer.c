#include "timer.h"
#include "led.h"

//��ʱ��3�жϷ������	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
//		steering_pwm_change();			    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);                                                                              //����жϱ�־λ 	    
}
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ42M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ��	  
	TIM3->DIER|=1<<0;   //��������ж�	  
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);	//��ռ1�������ȼ�3����2									 
}



void TIM2_Int_Init(u16 arr,u16 psc)
{
//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1; 	//TIM2ʱ��ʹ��    
	RCC->AHB1ENR|=1;   	//ʹ��PORTAʱ��	
	RCC->AHB1ENR|=1<<1;   	//ʹ��PORTBʱ��	
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_Set(GPIOB,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_Set(GPIOA,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_Set(GPIOA,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_AF_Set(GPIOA,5,1);	//PA0,AF1
	GPIO_AF_Set(GPIOB,3,1);	//PA1,AF1
	GPIO_AF_Set(GPIOA,2,1);	//PA2,AF1
	GPIO_AF_Set(GPIOA,3,1);	//PA2,AF1
	
	TIM2->ARR=arr;			//�趨�������Զ���װֵ 
	TIM2->PSC=psc;			//Ԥ��Ƶ������Ƶ 
	TIM2->CCMR1|=6<<4;  	//CH1 PWM1ģʽ	
	TIM2->CCMR1|=6<<12;  	//CH2 PWM1ģʽ	
	TIM2->CCMR2|=6<<4;  	//CH3 PWM1ģʽ
	TIM2->CCMR2|=6<<12;  	//CH3 PWM1ģʽ
	
	TIM2->CCMR1|=1<<3; 	//CH1 Ԥװ��ʹ��	
	TIM2->CCMR1|=1<<11; 	//CH2 Ԥװ��ʹ��	
	TIM2->CCMR2|=1<<3; 	//CH3 Ԥװ��ʹ��
	TIM2->CCMR2|=1<<11; 	//CH3 Ԥװ��ʹ��
	
	TIM2->CCER|=1<<0;   	//OC1 ���ʹ��	
	TIM2->CCER|=1<<4;   	//OC1 ���ʹ��
	TIM2->CCER|=1<<8;   	//OC1 ���ʹ��
	TIM2->CCER|=1<<12;   	//OC1 ���ʹ��
  	
//	TIM2->CCER|=0<<1;   	//OC1 �͵�ƽ��Ч	   
	TIM2->CR1|=1<<7;   		//ARPAʹ�� 
	TIM2->CR1|=1<<0;    	//ʹ�ܶ�ʱ��2 	
}




void TIM5_Int_Init(u16 arr,u16 psc)
{
//�˲������ֶ��޸�IO������
	RCC->APB1ENR|=1<<3; 	//TIM5ʱ��ʹ��    
	RCC->AHB1ENR|=1;   	//ʹ��PORTHʱ��	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_Set(GPIOA,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//���ù���,�������
	GPIO_AF_Set(GPIOA,0,2);	//PB6,AF2
	GPIO_AF_Set(GPIOA,1,2);	//PB7,AF2
	
	TIM5->ARR=arr;			//�趨�������Զ���װֵ 
	TIM5->PSC=psc;			//Ԥ��Ƶ������Ƶ 
	TIM5->CCMR1|=6<<4;  	//CH1 PWM1ģʽ	
	TIM5->CCMR1|=6<<12;  	//CH2 PWM1ģʽ	
	
	TIM5->CCMR1|=1<<3; 	//CH1 Ԥװ��ʹ��	
	TIM5->CCMR1|=1<<11; 	//CH2 Ԥװ��ʹ��	
	
	TIM5->CCER|=1<<0;   	//OC1 ���ʹ��	
	TIM5->CCER|=1<<4;   	//OC1 ���ʹ��
  	
//	TIM2->CCER|=0<<1;   	//OC1 �͵�ƽ��Ч	   
	TIM5->CR1|=1<<7;   		//ARPAʹ�� 
	TIM5->CR1|=1<<0;    	//ʹ�ܶ�ʱ��2 	
}














