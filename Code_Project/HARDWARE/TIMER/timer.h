#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

#define SREET1	TIM2->CCR2
#define	SREET3	TIM5->CCR1
#define	SREET2  TIM5->CCR2
#define	SREET4  TIM2->CCR4
#define	SREET5  TIM2->CCR1
#define	SREET6  TIM2->CCR3

void TIM5_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
#endif



















