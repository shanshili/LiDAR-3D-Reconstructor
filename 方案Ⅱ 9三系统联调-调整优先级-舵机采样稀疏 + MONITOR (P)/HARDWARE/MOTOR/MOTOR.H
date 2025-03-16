
#ifndef __MOTOR1_H
#define __MOTOR1_H
 
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
 
#define High    0
#define Low     1
 
#define IN1(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_2);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_2)
 
#define IN2(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_3);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_3)
 
#define IN3(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_4);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
 
#define IN4(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_5);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
 
void Motor_Config(void);
 
void Motor1(void);
void Motor2(void);
void Motor3(void);
void Motor4(void);
void Motor5(void);
void Motor6(void);
void Motor7(void);
void Motor8(void);
void FR(void);//È¥
void RF(void);//À´
u8 MRESET(void);
 
#endif
