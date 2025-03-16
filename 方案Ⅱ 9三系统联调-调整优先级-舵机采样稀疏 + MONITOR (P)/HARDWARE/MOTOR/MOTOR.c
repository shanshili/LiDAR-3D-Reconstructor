#include "motor.h"  //导入led头文件
#include "stm32f10x.h"  //导入STM32官方库
#include "stm32f10x_rcc.h"  //导入STM32的RCC时钟库
#include "delay.h"
#include "usart.h"
#include "display.h"
#include "oled.h"

#define TIME 3
#define min 100
#define max 240
u8 far[] = "fa";
u8 close[] = "cl";
u8 OK[] = "ok";
 
void Motor_Config(void)  //定义初始化函数
{
	GPIO_InitTypeDef GPIO_InitStructure;   //定义GPIO_InitTypeDef结构体
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //开启引脚时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; //定义IN引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //通用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //设置输出功率
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //初始化GPIOA的引脚参数,写进
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5); //所有引脚拉低
}
 

void Motor1(void)
{
	IN1(High);
	IN2(Low);
	IN3(Low);
	IN4(Low);
}
 
void Motor2(void)
{
	IN1(High);
	IN2(High);
	IN3(Low);
	IN4(Low);
}

void Motor3(void)
{
	IN1(Low);
	IN2(High);
	IN3(Low);
	IN4(Low);
}

void Motor4(void)
{
	IN1(Low);
	IN2(High);
	IN3(High);
	IN4(Low);
}

void Motor5(void)
{
	IN1(Low);
	IN2(Low);
	IN3(High);
	IN4(Low);
}

void Motor6(void)
{
	IN1(Low);
	IN2(Low);
	IN3(High);
	IN4(High);
}

void Motor7(void)
{
	IN1(Low);
	IN2(Low);
	IN3(Low);
	IN4(High);
}

void Motor8(void)
{
	IN1(High);
	IN2(Low);
	IN3(Low);
	IN4(High);
}

void FR(void)
{
  Motor2();
	delay_ms(TIME);
	//Motor2();delay_ms(TIME);
	Motor4();delay_ms(TIME);
	//Motor4();delay_ms(TIME);
	Motor6();delay_ms(TIME);
	//Motor6();delay_ms(TIME);
	Motor8();delay_ms(TIME);
	//Motor8();delay_ms(TIME);
}

void RF(void)
{
  //Motor8();delay_ms(TIME);
	Motor8();delay_ms(TIME);
	//Motor6();delay_ms(TIME);
	Motor6();delay_ms(TIME);
	//Motor4();delay_ms(TIME);
	Motor4();delay_ms(TIME);
	//Motor2();delay_ms(TIME);
	Motor2();delay_ms(TIME);
}

u8 MRESET(void)
{
		display_Data(Monitor_Distance);
	  delay_ms(1000);	delay_ms(1000);	delay_ms(1000);
	 do{
					if ((Monitor_Distance > (min+7) ) && (Monitor_Distance < (max) ) )
					{		
							RF();		//向后	  	
							OLED_ShowString(96,0,far,16);			
							display_Data(Monitor_Distance);
						 // break;//问题所在
					}
					if (Monitor_Distance < min)
					{	  
							FR();					
							OLED_ShowString(72,0,close,16);
							display_Data(Monitor_Distance);
					}
					if (Monitor_Distance > max)
					{	  
							RF();	
							OLED_ShowString(48,0,far,16);				
							display_Data(Monitor_Distance);
					}
	}
	while((Monitor_Distance > (min+7)) || (Monitor_Distance < min) );
	OLED_ShowString(96,0,OK,16);	
	//OLED_Clear();
	return 1;
}




