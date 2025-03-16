
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
#define INY1(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_6);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_6)
 
#define INY2(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_7);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_7)
 
#define INY3(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_8);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_8)
 
#define INY4(a) if (a)  \
	GPIO_SetBits(GPIOC,GPIO_Pin_9);\
else  \
	GPIO_ResetBits(GPIOC,GPIO_Pin_9)
 
void Motor_Config(void);
 
void Motor1(void);
void Motor2(void);
void Motor3(void);
void Motor4(void);
void Motor5(void);
void Motor6(void);
void Motor7(void);
void Motor8(void);
void MotorY2(void);
void MotorY4(void);
void MotorY6(void);
void MotorY8(void);
void FR(void);//去
void RF(void);//来
void YFR(void);//去
void YRF(void);//来
u8 MRESET(void);
void Motor_reset(void);
void MotorY_reset(void);
 
#endif
