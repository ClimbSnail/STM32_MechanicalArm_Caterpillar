#include "sys.h"
#include "usart.h"	
#include "led.h"
#include "timer.h"
#include "steering.h"
#include "my_gpio.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

u8 data;     //用于串口1接适蒈
u8 receive_flag=0;   //串口接收标志
float x=10,z=150;//实际坐标点
float x_start,z_start,x_end,z_end;//起始/终止坐标
float SREET6_start,SREET6_end;
int setting_flag = 0;//记忆功能的标志
u16 SREET1_start,SREET1_end; //保存夹头开始和终止坐标
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 
//end
//////////////////////////////////////////////////////////////////

#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  

void USART1_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
    OSIntEnter();
#endif
    if(USART1->SR&(1<<5))//接收到数据
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
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
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
            LED1=0;//向前
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
            LED1=0;//向后
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
            LED1=0;//向上
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
            LED1=0;//向下
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
            if(SREET6>=2497)SREET6 = 2497;//逆时针
            SREET6 += 1;
            delay_ms1(500);
            break;
        case 0x06:
            if(SREET6<=503)SREET6 = 503;//顺时针
            SREET6 -= 1;
            delay_ms1(500);
            break;
        case 0x07:
            if(SREET1>=2497)SREET1 = 2497;//夹紧
            SREET1 += 2;
            delay_ms1(500);
            break;
        case 0x08:
            if(SREET1<=1503)SREET1 = 1503;//松开
            SREET1 -= 2;
            delay_ms1(500);
            break;
        case 0x09:
            LED1=0;//0x92前进
            CAR1=1;
            CAR2=0;
            CAR3=1;
            CAR4=0;
            delay_ms1(500);
            break;
        case 0x0a:
            LED1=0;//0x94后退
            CAR1=0;
            CAR2=1;
            CAR3=0;
            CAR4=1;
            delay_ms1(500);
            break;
        case 0x0b:
            LED1=0;//0x97顺拐
            CAR1=0;
            CAR2=1;
            CAR3=1;
            CAR4=0;
            delay_ms1(500);
            break;
        case 0x0c:
            LED1=0;//0x98逆拐
            CAR1=1;
            CAR2=0;
            CAR3=0;
            CAR4=1;
            delay_ms1(500);
            break;
        case 0x0d:
            SREET1_start = SREET1;//记忆机械臂
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
            setting_flag = 1;//启动记忆功能的标志
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

//初始化IO 串口1
//pclk2:PCLK2时钟频率(Mhz)
//bound:波特率 
void uart1_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//得到USARTDIV@OVER8=0
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分@OVER8=0 
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->AHB1ENR|=1<<1;   	//使能PORTB口时钟  
	RCC->APB2ENR|=1<<4;  	//使能串口1时钟 
	GPIO_Set(GPIOB,PIN6|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_50M,GPIO_PUPD_PU);//PB6,PB7,复用功能,上拉输出
 	GPIO_AF_Set(GPIOB,6,7);	//PB6,AF7
	GPIO_AF_Set(GPIOB,7,7);//PB7,AF7  	   
	//波特率设置
 	USART1->BRR=mantissa; 	//波特率设置	 
	USART1->CR1&=~(1<<15); 	//设置OVER8=0 
	USART1->CR1|=1<<3;  	//串口发送使能 
#if EN_USART1_RX		  	//如果使能了接收
	//使能接收中断 
	USART1->CR1|=1<<2;  	//串口接收使能
	USART1->CR1|=1<<5;    	//接收缓冲区非空中断使能	    	
	MY_NVIC_Init(3,3,USART1_IRQn,2);//组2，最低优先级 
#endif
	USART1->CR1|=1<<13;  	//串口使能
}