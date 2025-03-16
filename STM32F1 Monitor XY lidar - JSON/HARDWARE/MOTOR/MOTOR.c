#include "motor.h"  //����ledͷ�ļ�
#include "stm32f10x.h"  //����STM32�ٷ���
#include "stm32f10x_rcc.h"  //����STM32��RCCʱ�ӿ�
#include "delay.h"
#include "usart.h"
#include "display.h"
#include "oled.h"

#define TIME 3
#define min 100
#define max 370
u8 far[] = "fa";
u8 close[] = "cl";
u8 OK[] = "ok";
 
void Motor_Config(void)  //�����ʼ������
{
	GPIO_InitTypeDef GPIO_InitStructure;   //����GPIO_InitTypeDef�ṹ��
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);  //��������ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //����IN����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //ͨ���������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //�����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);  //��ʼ��GPIOA�����Ų���,д��
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9); //������������
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


void MotorY2(void)
{
	INY1(High);
	INY2(High);
	INY3(Low);
	INY4(Low);
}
void MotorY4(void)
{
	INY1(Low);
	INY2(High);
	INY3(High);
	INY4(Low);
}

void MotorY6(void)
{
	INY1(Low);
	INY2(Low);
	INY3(High); 
	INY4(High);
}

void MotorY8(void)
{
	INY1(High);
	INY2(Low);
	INY3(Low);
	INY4(High);
}

void FR(void)
{
  Motor1();delay_ms(TIME);
	Motor2();delay_ms(TIME);
	Motor3();delay_ms(TIME);
	Motor4();delay_ms(TIME);
	Motor5();delay_ms(TIME);
	Motor6();delay_ms(TIME);
	Motor7();delay_ms(TIME);
	Motor8();delay_ms(TIME);
}

void RF(void)
{
  Motor8();delay_ms(TIME);
	Motor7();delay_ms(TIME);
	Motor6();delay_ms(TIME);
	Motor5();delay_ms(TIME);
	Motor4();delay_ms(TIME);
	Motor3();delay_ms(TIME);
	Motor2();delay_ms(TIME);
	Motor1();delay_ms(TIME);
}

void YFR(void)
{
	MotorY2();delay_ms(TIME);
	MotorY4();delay_ms(TIME);
	MotorY6();delay_ms(TIME);
	MotorY8();delay_ms(TIME);
}

void YRF(void)
{
  MotorY8();delay_ms(TIME);
	MotorY6();delay_ms(TIME);
	MotorY4();delay_ms(TIME);
	MotorY2();delay_ms(TIME);
}

void Motor_reset(void)
{
	IN1(Low);
	IN2(Low);
	IN3(Low);
	IN4(Low);
}

void MotorY_reset(void)
{
	INY1(High);
	INY2(High);
	INY3(High);
	INY4(High);
}

//u8 MRESET(void)
//{
//	 display_Data(Active_Distance);
//	 do{ }
//	 while(Active_Distance == 0 );
//	 do{
//				if ((Active_Distance > (min+5) ) && (Active_Distance < (max-5) ) )
//				{		
//						RF();			  	
//						OLED_ShowString(96,0,OK,16);			
//						display_Data(Active_Distance);
//						break;
//				}
//				if (Active_Distance < min)
//				{	  
//						FR();					
//						OLED_ShowString(72,0,close,16);
//						display_Data(Active_Distance);
//				}
//				if (Active_Distance > max)
//				{	  
//						RF();	
//						OLED_ShowString(48,0,far,16);				
//						display_Data(Active_Distance);
//				}
//		}
//	 while((Active_Distance > (min+5)) || (Active_Distance < min) );
//	 //OLED_Clear();
//	return 1;
//}


