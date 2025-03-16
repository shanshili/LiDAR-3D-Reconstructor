#ifndef _TFT_144_H_
#define _TFT_144_H_

#include "stm32f10x_conf.h"
#include <stdlib.h>

/* parameters */
#define WIDTH  128
#define LENGTH 128


/* pins definitions */
#define LA0	GPIO_Pin_6		// GPIOC
#define LSDI GPIO_Pin_15	// GPIOB
#define LSCL GPIO_Pin_13	// GPIOB
#define LCS GPIO_Pin_12		// GPIOB

#define LA0_SET() GPIOC->BSRR = 1<<6
#define LA0_CLR() GPIOC->BRR = 1<<6

#define LCS_SET() GPIOB->BSRR = 1<<12
#define LCS_CLR() GPIOB->BRR = 1<<12

/* functions */
void TFT_GPIO ( void );		// TFT��GPIO��ʼ��
void TFT_MEM  ( void );		// �Դ�ķ���
void TFT_INIT ( void );		// TFT��ʼ��

#define LCMD 0		// ����
#define LDAT 1		// ����
void writeCmdData ( unsigned char Byte, unsigned char opt );

#define RED		0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define YELLOW	0xffe0
#define BLACK	0x0000
#define WHITE	0xffff
void setPos( int sx, int ex, int sy, int ey );	// ���û�ͼ����
void write2Byte( unsigned int twoByte );		// д�����ֽڣ���Ϊcolor������16λ
void writeAnColor( unsigned int color );		// ���ȫ��

#endif


