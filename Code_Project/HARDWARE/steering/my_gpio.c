#include "my_gpio.h"
#include "sys.h"

void my_gpioinit(void)
{
	RCC->AHB1ENR|=1<<6;   	//使能PORTG时钟	
	RCC->AHB1ENR|=1<<1; 		//使能PORTB时钟	
	RCC->AHB1ENR|=1<<4; 		//使能PORTE时钟	
	GPIO_Set(GPIOG,PIN15,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//推挽输出,上拉输出    G15
	GPIO_Set(GPIOB,PIN8|PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//推挽输出,上拉输出  B4 B5
	GPIO_Set(GPIOE,PIN0,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU);//推挽输出,上拉输出     E0
	CAR1=0;
	CAR2=0;
	CAR3=0;
	CAR4=0;
}





















