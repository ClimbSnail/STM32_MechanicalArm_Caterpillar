#include "wdg.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//���Ź� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved				
//********************************************************************************
//V1.1 20140504
//�����˴��ڿ��Ź���غ�����
////////////////////////////////////////////////////////////////////////////////// 	 

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).
void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG->PR=prer;  //���÷�Ƶϵ��   
  	IWDG->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//ʹ�ܿ��Ź�	
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}

//����WWDG������������ֵ,Ĭ��Ϊ���. 
u8 WWDG_CNT=0x7f; 
//��ʼ�����ڿ��Ź� 	
//tr   :T[6:0],������ֵ 
//wr   :W[6:0],����ֵ 
//fprer:��Ƶϵ����WDGTB��,�����2λ��Ч 
//Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=42Mhz
void WWDG_Init(u8 tr,u8 wr,u8 fprer) 
{    
	RCC->APB1ENR|=1<<11; 	//ʹ��wwdgʱ�� 
	WWDG_CNT=tr&WWDG_CNT;   //��ʼ��WWDG_CNT.     
	WWDG->CFR|=fprer<<7;    //PCLK1/4096�ٳ�2^fprer 
	WWDG->CFR&=0XFF80;      
	WWDG->CFR|=wr;     		//�趨����ֵ      
	WWDG->CR|=WWDG_CNT; 	//�趨������ֵ 
	WWDG->CR|=1<<7;  		//�������Ź�      
	MY_NVIC_Init(2,3,WWDG_IRQn,2);//��ռ2�������ȼ�3����2     
	WWDG->SR=0X00; 			//�����ǰ�����жϱ�־λ 
	WWDG->CFR|=1<<9;        //ʹ����ǰ�����ж� 
} 
//������WWDG��������ֵ 
void WWDG_Set_Counter(u8 cnt) 
{ 
	WWDG->CR =(cnt&0x7F);//������7λ������ 
} 
//���ڿ��Ź��жϷ������ 
void WWDG_IRQHandler(void) 
{      
	WWDG_Set_Counter(WWDG_CNT);//���贰�ڿ��Ź���ֵ!         
	WWDG->SR=0X00;//�����ǰ�����жϱ�־λ 
	LED1=!LED1; 
}







