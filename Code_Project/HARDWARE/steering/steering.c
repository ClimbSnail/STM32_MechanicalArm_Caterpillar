#include "stm32f4xx.h"
#include "steering.h"
#include "timer.h"
#include "led.h"
#define PI 3.14
//???????
unsigned char mark[6] = {0x00,0x00,0x00};//pwm???1,???0
u16 now_pwm_duty[]= {1500,1500,1500}; //???pwm
u16 target_pwm_duty[]= {1500,1500,1500}; //??pwm
u16 cos_pwm[180];//?????????pwm
//??????
//float triangle1_c1=10.5;
//float triangle1_c2=9.7*0.411;//97?????? 0.411???????????
//float triangle1_c3;
//??????
//float triangle2_c1=15.0;
//float triangle2_c2=9.7*0.589;//97?????? 0.589???????????
//float triangle2_c3;
void Init_cos_pwm(void);
void Change_angle(float x,float z);//?????,????x,z??????????????
//
void steering_pwm_change1(void);
float Sqrt_2(float d);	//?????(?????)
float Sqrt_1(float x);	//??????????
float pow_2(float x);	//?????
int Seach_angle(float cos);	//?????????????????
void steering_pwm_change(void);	//??????
//
void delay_ms1(unsigned int z)
{
    unsigned int v;
    for( ; z>0; z--)
        for(v=100; v>0; v--);
}
void steering_init(void)//???
{
    Init_cos_pwm();
}

float cos_form[181]=  	//1~90???????
{
    0.99985,0.99939,0.99863,0.99756,0.99619,0.99452,0.99255,0.99027,0.98769,0.98481,
    0.98163,0.97815,0.97437,0.97030,0.96593,0.96126,0.95630,0.95106,0.94552,0.93969,
    0.93358,0.92718,0.92050,0.91355,0.90631,0.89879,0.89101,0.88295,0.87462,0.86603,
    0.85717,0.84805,0.83867,0.82904,0.81915,0.80902,0.79864,0.78801,0.77715,0.76604,
    0.75471,0.74314,0.73135,0.71934,0.70711,0.69466,0.68200,0.66913,0.65606,0.64279,
    0.62932,0.61566,0.60182,0.58779,0.57358,0.55919,0.54464,0.52992,0.51504,0.50000,
    0.48481,0.46947,0.45399,0.43837,0.42262,0.40674,0.39073,0.37461,0.35837,0.34202,
    0.32557,0.30902,0.29237,0.27564,0.25882,0.24192,0.22495,0.20791,0.19081,0.17365,
    0.15643,0.13917,0.12187,0.10453,0.08716,0.06976,0.05234,0.03490,0.01745,0,

    -0.01745,-0.03490,-0.05234,-0.06976,-0.08716,-0.10453,-0.12187,-0.13917,-0.15643,
    -0.17365,-0.19081,-0.20791,-0.22495,-0.24192,-0.25882,-0.27564,-0.29237,-0.30902,-0.32557,
    -0.34202,-0.35837,-0.37461,-0.39073,-0.40674,-0.42262,-0.43837,-0.45399,-0.46947,-0.48481,
    -0.50000,-0.51504,-0.52992,-0.54464,-0.55919,-0.57358,-0.58779,-0.60182,-0.61566,-0.62932,
    -0.64279,-0.65606,-0.66913,-0.68200,-0.69466,-0.70711,-0.71934,-0.73135,-0.74314,-0.75471,
    -0.76604,-0.77715,-0.78801,-0.79864,-0.80902,-0.81915,-0.82904,-0.83867,-0.84805,-0.85717,
    -0.86603,-0.87462,-0.88295,-0.89101,-0.89879,-0.90631,-0.91355,-0.92050,-0.92718,-0.93358,
    -0.93969,-0.94552,-0.95106,-0.95630,-0.96126,-0.96593,-0.97030,-0.97437,-0.97815,-0.98163,
    -0.98481,-0.98769,-0.99027,-0.99255,-0.99452,-0.99619,-0.99756,-0.99863,-0.99939,-0.99985,
    -1

};

void Init_cos_pwm(void)//???cos_pwm??
{
    int i;
//    for( i = 180 ; i>0 ; i-- )
    for( i = 1 ; i<181 ; i++ )
        cos_pwm[i-1] = 500+i*11.11;//??pwm???????
}

float Sqrt_2(float dou)//?????(?????)
{
    double x,y;
    x=0.0;
    y=dou/2;
    while(x!=y)
    {
        x=y;
        y=(x+dou/x)/2;
    }
    return x;
}

float Sqrt_1(float x)	//??????????
{
    float xhalf = 0.5f*x;
    int i = *(int *)&x;
    i = 0x5f375a86-(i>>1);
    x = *(float *)&i;
    x = x*(1.5f-xhalf*x*x);
    x = x*(1.5f-xhalf*x*x);
    x = x*(1.5f-xhalf*x*x);
    return 1/x;
}

float pow_2(float x)//?????
{
    return x*x;
}

int Seach_angle(float cos) //?????????????????
{
    int l=1,r=180;
    int middle;
    while(l<r)
    {
        middle = (l+r)/2;
        if( cos < cos_form[middle] )
            l = middle + 1;
        else if(cos > cos_form[middle])
            r = middle - 1;
        else return middle;
    }
    return r;
}

void Change_angle(float x,float z)//?????,????x,z??????????????
{
    if( z >= 0 )
    {
        float arm_long=Sqrt_2(x*x+z*z);//?????
        float cosa = x/arm_long;
        float cosb = (105*105+arm_long*arm_long-97*97)/2/105/arm_long;
        float cosc = (97*97+arm_long*arm_long-105*105)/2/97/arm_long;

        int angle_a = Seach_angle(cosa);
        int angle_b = Seach_angle(cosb);
        int angle_c = Seach_angle(cosc);
        //???????pwm?
        target_pwm_duty[0] = cos_pwm[90 - (angle_a - angle_c)];
        target_pwm_duty[1] = cos_pwm[90 - (angle_b + angle_c)];
        target_pwm_duty[2] = cos_pwm[180- angle_a - angle_b ] ;

        steering_pwm_change1();
    }
    else
    {
        float arm_long=Sqrt_2(x*x+z*z);
        float cosa = x/arm_long;
        float cosb = (arm_long*arm_long+97*97-160*160)/2/97/arm_long;
        float cosc = (97*97+160*160-arm_long*arm_long)/2/97/160;

        int angle_a = Seach_angle(cosa);
        int angle_b = Seach_angle(cosb);
        int angle_c = Seach_angle(cosc);
        target_pwm_duty[0] = cos_pwm[ angle_c - 90 ];
        target_pwm_duty[1] = cos_pwm[90 + (angle_b - angle_a)];

        SREET3 = target_pwm_duty[0];
        SREET4 = target_pwm_duty[1];
        SREET5 = 2500;
    }
}
void steering_pwm_change1(void)
{
    SREET3 = target_pwm_duty[0];
    SREET4 = target_pwm_duty[1];
    SREET5 = target_pwm_duty[2];
}
void steering_pwm_change(void)//??????
{
    u16 a=50;//????pwm??? ?1?pwm??92 10?????10?
    int i;
    for( i=0 ; i<3 ; i++ )
    {
        if( mark[i]==0x02 && now_pwm_duty[i]<target_pwm_duty[i] )
        {
            now_pwm_duty[i] += a;
            if( now_pwm_duty[i]>=target_pwm_duty[i] )
            {
                now_pwm_duty[i] = target_pwm_duty[i];
                mark[i] = 0x00;
            }
            switch (i)
            {
            case 0:
                SREET3 = now_pwm_duty[0];
                break;
            case 1:
                SREET4 = now_pwm_duty[1];
                break;
            case 2:
                SREET5 = now_pwm_duty[2];
                break;
            default:
                break;
            }
        }

        else if( mark[i]==0x01 && now_pwm_duty[i]>target_pwm_duty[i] )
        {
            now_pwm_duty[i] -= a;
            if( now_pwm_duty[i]<=target_pwm_duty[i] )
            {
                now_pwm_duty[i] = target_pwm_duty[i];
                mark[i] = 0x00;
            }
            switch (i)
            {
            case 0:
                SREET3 = now_pwm_duty[0];
                break;
            case 1:
                SREET4 = now_pwm_duty[1];
                break;
            case 2:
                SREET5 = now_pwm_duty[2];
                break;
            default:
                break;
            }
        }
    }

}
