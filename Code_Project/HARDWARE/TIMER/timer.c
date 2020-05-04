#include "timer.h"
#include "led.h"

//定时器3中断服务程序	 
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
//		steering_pwm_change();			    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);                                                                              //清除中断标志位 	    
}
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为42M
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器	  
	TIM3->DIER|=1<<0;   //允许更新中断	  
	TIM3->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM3_IRQn,2);	//抢占1，子优先级3，组2									 
}



void TIM2_Int_Init(u16 arr,u16 psc)
{
//此部分需手动修改IO口设置
	RCC->APB1ENR|=1; 	//TIM2时钟使能    
	RCC->AHB1ENR|=1;   	//使能PORTA时钟	
	RCC->AHB1ENR|=1<<1;   	//使能PORTB时钟	
	GPIO_Set(GPIOA,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_Set(GPIOB,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_Set(GPIOA,PIN2,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_Set(GPIOA,PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_AF_Set(GPIOA,5,1);	//PA0,AF1
	GPIO_AF_Set(GPIOB,3,1);	//PA1,AF1
	GPIO_AF_Set(GPIOA,2,1);	//PA2,AF1
	GPIO_AF_Set(GPIOA,3,1);	//PA2,AF1
	
	TIM2->ARR=arr;			//设定计数器自动重装值 
	TIM2->PSC=psc;			//预分频器不分频 
	TIM2->CCMR1|=6<<4;  	//CH1 PWM1模式	
	TIM2->CCMR1|=6<<12;  	//CH2 PWM1模式	
	TIM2->CCMR2|=6<<4;  	//CH3 PWM1模式
	TIM2->CCMR2|=6<<12;  	//CH3 PWM1模式
	
	TIM2->CCMR1|=1<<3; 	//CH1 预装载使能	
	TIM2->CCMR1|=1<<11; 	//CH2 预装载使能	
	TIM2->CCMR2|=1<<3; 	//CH3 预装载使能
	TIM2->CCMR2|=1<<11; 	//CH3 预装载使能
	
	TIM2->CCER|=1<<0;   	//OC1 输出使能	
	TIM2->CCER|=1<<4;   	//OC1 输出使能
	TIM2->CCER|=1<<8;   	//OC1 输出使能
	TIM2->CCER|=1<<12;   	//OC1 输出使能
  	
//	TIM2->CCER|=0<<1;   	//OC1 低电平有效	   
	TIM2->CR1|=1<<7;   		//ARPA使能 
	TIM2->CR1|=1<<0;    	//使能定时器2 	
}




void TIM5_Int_Init(u16 arr,u16 psc)
{
//此部分需手动修改IO口设置
	RCC->APB1ENR|=1<<3; 	//TIM5时钟使能    
	RCC->AHB1ENR|=1;   	//使能PORTH时钟	
	GPIO_Set(GPIOA,PIN0,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_Set(GPIOA,PIN1,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//复用功能,上拉输出
	GPIO_AF_Set(GPIOA,0,2);	//PB6,AF2
	GPIO_AF_Set(GPIOA,1,2);	//PB7,AF2
	
	TIM5->ARR=arr;			//设定计数器自动重装值 
	TIM5->PSC=psc;			//预分频器不分频 
	TIM5->CCMR1|=6<<4;  	//CH1 PWM1模式	
	TIM5->CCMR1|=6<<12;  	//CH2 PWM1模式	
	
	TIM5->CCMR1|=1<<3; 	//CH1 预装载使能	
	TIM5->CCMR1|=1<<11; 	//CH2 预装载使能	
	
	TIM5->CCER|=1<<0;   	//OC1 输出使能	
	TIM5->CCER|=1<<4;   	//OC1 输出使能
  	
//	TIM2->CCER|=0<<1;   	//OC1 低电平有效	   
	TIM5->CR1|=1<<7;   		//ARPA使能 
	TIM5->CR1|=1<<0;    	//使能定时器2 	
}














