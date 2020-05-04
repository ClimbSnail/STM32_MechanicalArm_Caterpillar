#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "timer.h"
#include "steering.h"
#include "my_gpio.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

u8 data;     //���ڴ���1��������
u8 receive_flag=0;   //���ڽ��ձ�־
float x=10,z=150;//ʵ�������
float x_start,z_start,x_end,z_end;//��ʼ/��ֹ����
float SREET6_start,SREET6_end;
int setting_flag = 0;//���书�ܵı�־
u16 SREET1_start,SREET1_end; //�����ͷ��ʼ����ֹ����
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef�� d in stdio.h. */ 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  

void USART1_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntEnter();
#endif
    if(USART1->SR&(1<<5))//���յ�����
    {
        u8 count = 0;
        u8 temp;
        int i=0;
        data  =   USART1->DR;
        if( data >= 128 )
        {
            temp = data;
            do
            {
                if( temp & 0x01 )
                {
                    count++;
                }
                temp >>= 1;
            }
            while( i++<7 );

            if( count % 2 )
            {
                data >>= 1;
                data &= 0x0f;
                receive_flag = 1;
            }
        }
        if( data != 0x0e )
            setting_flag = 0;
    }
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
    OSIntExit();
#endif
}
#endif


void RECEIVE_JUGE(void)
{
    if( receive_flag == 1 )
    {
        switch( data )
        {
        case 0x00:          //    LED1=0;
            receive_flag = 0;
            CAR1=0;
            CAR2=0;
            CAR3=0;
            CAR4=0;
            break;
        case 0x01:
            LED1=0;//��ǰ
            if( x*x+z*z<=40800 && x<200 && z>0)
            {
                if( x*x+z*z>=38000 && z>=0.1f)
                    z -= 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z>40800 && z>0 )
                x-=0.1f;
            else if( x*x+z*z<=65025 && z<=0)
            {
                if( x*x+z*z>=64025 && z<=-0.1f)
                    z += 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
            break;
        case 0x02:
            LED1=0;//���
            if( x*x+z*z>=20500 && x>1)
            {
                if( x*x+z*z<=22000 && z<200 )
                    z += 0.1f;
                x-=0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z<20500 )
                x+=0.1f;
            break;
        case 0x03:
            LED1=0;//����
            if( z>0 )
            {
                if( x*x+z*z<40800 )
                {
                    if( x*x+z*z>=38000 && x>1 )
                        x-=0.1f;
                    z+=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z>=40800 )
                {
                    z-=0.1f;
                }
            }
            else
            {
                if( x*x+z*z<36000 && x<200)
                    x+=0.1f;
                z+=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z-=0.1f;
            }
            break;
        case 0x04:
            LED1=0;//����
            if( z>0 )
            {
                if( x*x+z*z>20434 )
                {
                    if( x*x+z*z<23000 && x<200)
                        x+=0.1f;
                    z-=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z<=20434 )
                    z+=0.1f;
            }
            else
            {
                if( x*x+z*z<36000 && x>1)
                    x+=0.1f;
                z-=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z+=0.1f;
            }
            break;
        case 0x05:
            if(SREET6>=2497)SREET6 = 2497;//��ʱ��
            SREET6 += 1;
            delay_ms1(500);
            break;
        case 0x06:
            if(SREET6<=503)SREET6 = 503;//˳ʱ��
            SREET6 -= 1;
            delay_ms1(500);
            break;
        case 0x07:
            if(SREET1>=2497)SREET1 = 2497;//�н�
            SREET1 += 2;
            delay_ms1(500);
            break;
        case 0x08:
            if(SREET1<=1503)SREET1 = 1503;//�ɿ�
            SREET1 -= 2;
            delay_ms1(500);
            break;
        case 0x09:
            LED1=0;//0x92ǰ��
            CAR1=1;
            CAR2=0;
            CAR3=1;
            CAR4=0;
            delay_ms1(500);
            break;
        case 0x0a:
            LED1=0;//0x94����
            CAR1=0;
            CAR2=1;
            CAR3=0;
            CAR4=1;
            delay_ms1(500);
            break;
        case 0x0b:
            LED1=0;//0x97˳��
            CAR1=0;
            CAR2=1;
            CAR3=1;
            CAR4=0;
            delay_ms1(500);
            break;
        case 0x0c:
            LED1=0;//0x98���
            CAR1=1;
            CAR2=0;
            CAR3=0;
            CAR4=1;
            delay_ms1(500);
            break;
        case 0x0d:
            SREET1_start = SREET1;//�����е��
            SREET6_start = SREET6;
            x_start = x;
            z_start = z;
            receive_flag = 0;
            break;
        case 0x0e:
            SREET1_end = SREET1;
            SREET6_end = SREET6;
            x_end = x;
            z_end = z;
            LED1=0;
            setting_flag = 1;//�������书�ܵı�־
            receive_flag = 0;
            while( setting_flag )
            ative_auto();
            break;
        case 0x0f:
            SREET1_start = 2497;
            SREET6_start = 2000;
            x_start = 10;
            z_start = 150;
            SREET1_end = 1503;
            SREET6_end = 1000;
            x_end = 80;
            z_end = 80;
            receive_flag = 0;
            ative_auto();
            break;
        default:
            break;
        }
        delay_ms1(350);
    }
}

void ative_auto()
{
    LED0=0;
		while( SREET6 != SREET6_start )
		{
			if( SREET6 < SREET6_start )
				SREET6 += 1;
			else SREET6 -= 1;
			delay_ms1(600);
			if( ( SREET6>SREET6_start && SREET6 - SREET6_start < 1) || ( SREET6<SREET6_start && SREET6_start - SREET6 < 1) )
				SREET6 = SREET6_start;
		}
    while(  x!=x_start || z!=z_start)
    {
        if( x<x_start )
        {
            if( x*x+z*z<=40800 && x<200 && z>0)
            {
                if( x*x+z*z>=38000 && z>=0.1f)
                    z -= 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z>40800 && z>0 )
                x-=0.1f;
            else if( x*x+z*z<=65025 && z<=0)
            {
                if( x*x+z*z>=64025 && z<=-0.1f)
                    z += 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
        }
        else if( x>x_start )
        {
            if( x*x+z*z>=20500 && x>1)
            {
                if( x*x+z*z<=22000 && z<200 )
                    z += 0.1f;
                x-=0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z<20500 )
                x+=0.1f;
        }
        if( z<z_start )
        {
            if( z>0 )
            {
                if( x*x+z*z<40800 )
                {
                    if( x*x+z*z>=38000 && x>1 )
                        x-=0.1f;
                    z+=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z>=40800 )
                {
                    z-=0.1f;
                }
            }
            else
            {
                if( x*x+z*z<36000 && x<200)
                    x+=0.1f;
                z+=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z-=0.1f;
            }
        }
        else if( z>z_start )
        {
            if( z>0 )
            {
                if( x*x+z*z>20434 )
                {
                    if( x*x+z*z<23000 && x<200)
                        x+=0.1f;
                    z-=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z<=20434 )
                    z+=0.1f;
            }
            else
            {
                if( x*x+z*z<36000 && x>1)
                    x+=0.1f;
                z-=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z+=0.1f;
            }
        }
        //	 Change_angle(x,z);
        delay_ms1(650);
    }
		while( SREET1 != SREET1_start )
		{
			if( SREET1 < SREET1_start )
				SREET1 += 1;
			else SREET1 -= 1;
			delay_ms1(600);
			if( ( SREET1>SREET1_start && SREET1 - SREET1_start < 1) || ( SREET1<SREET1_start && SREET1_start - SREET1 < 1) )
				SREET1 = SREET1_start;
		}
		
		while( SREET6 != SREET6_end )
		{
			if( SREET6 < SREET6_end )
				SREET6 += 1;
			else SREET6 -= 1;
			delay_ms1(600);
			if( ( SREET6>SREET6_end && SREET6 - SREET6_end < 1) || ( SREET6<SREET6_end && SREET6_end - SREET6 < 1) )
				SREET6 = SREET6_end;
		}
    while( x!=x_end || z!=z_end )
    {
        if( x<x_end )
        {
            if( x*x+z*z<=40800 && x<200 && z>0)
            {
                if( x*x+z*z>=38000 && z>=0.1f)
                    z -= 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z>40800 && z>0 )
                x-=0.1f;
            else if( x*x+z*z<=65025 && z<=0)
            {
                if( x*x+z*z>=64025 && z<=-0.1f)
                    z += 0.1f;
                x += 0.1f;
                Change_angle(x,z);
            }
        }
        else if( x>x_end )
        {
            if( x*x+z*z>=20500 && x>1)
            {
                if( x*x+z*z<=22000 && z<200 )
                    z += 0.1f;
                x-=0.1f;
                Change_angle(x,z);
            }
            else if( x*x+z*z<20500 )
                x+=0.1f;
        }
        if( z<z_end )
        {
            if( z>0 )
            {
                if( x*x+z*z<40800 )
                {
                    if( x*x+z*z>=38000 && x>1 )
                        x-=0.1f;
                    z+=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z>=40800 )
                {
                    z-=0.1f;
                }
            }
            else
            {
                if( x*x+z*z<36000 && x<200)
                    x+=0.1f;
                z+=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z-=0.1f;
            }
        }
        else if( z>z_end )
        {
            if( z>0 )
            {
                if( x*x+z*z>20434 )
                {
                    if( x*x+z*z<23000 && x<200)
                        x+=0.1f;
                    z-=0.1f;
                    Change_angle(x,z);
                }
                else if( x*x+z*z<=20434 )
                    z+=0.1f;
            }
            else
            {
                if( x*x+z*z<36000 && x>1)
                    x+=0.1f;
                z-=0.1f;
                Change_angle(x,z);
                if( x*x+z*z<=35000 )
                    z+=0.1f;
            }
        }		
//		 Change_angle(x,z);
        delay_ms1(650);
    }
		while( SREET1 != SREET1_end )
		{
			if( SREET1 < SREET1_end )
				SREET1 += 1;
			else SREET1 -= 1;
			delay_ms1(600);
			if( ( SREET1>SREET1_end && SREET1 - SREET1_end < 1) || ( SREET1<SREET1_end && SREET1_end - SREET1 < 1) )
				SREET1 = SREET1_end;
		}
}

//��ʼ��IO ����1
//pclk2:PCLK2ʱ��Ƶ��(Mhz)
//bound:������ 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//�õ�USARTDIV@OVER8=0
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//ʹ��PORTB��ʱ��  
	RCC->APB2ENR|=1<<4;  	//ʹ�ܴ���1ʱ�� 
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB6,PB7,���ù���,�������
 	GPIO_AF_Set(GPIOB,6,7);	//PB6,AF7
	GPIO_AF_Set(GPIOB,7,7);//PB7,AF7  	   
	//����������
 	USART1->BRR=mantissa; 	//����������	 
	USART1->CR1&=~(1<<15); 	//����OVER8=0 
	USART1->CR1|=1<<3;  	//���ڷ���ʹ�� 
#if EN_USART1_RX		  	//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART1->CR1|=1<<2;  	//���ڽ���ʹ��
	USART1->CR1|=1<<5;    	//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//��2��������ȼ� 
#endif
	USART1->CR1|=1<<13;  	//����ʹ��
}