#ifndef __my_gpio_H
#define __my_gpio_H	 
#include "sys.h" 

//LED¶Ë¿Ú¶¨Òå
#define CAR1 PEout(0)	
#define CAR2 PBout(8)	
#define CAR3 PBout(4)	
#define CAR4 PGout(15)	

void my_gpioinit(void);				    
#endif


