#ifndef _STEERING_H_
#define _STEERING_H_
#include "stm32f4xx.h"
//extern u16 target_pwm_duty[6];
extern float cos_form[181];
//extern void steering_pwm_change();//??????
//
extern void steering_init(void);//???
extern void change_target_pwm_duty(int i,u8 turn,u16 value);
extern void Change_angle(float x,float z);
extern void delay_ms1(unsigned int z);
#endif


