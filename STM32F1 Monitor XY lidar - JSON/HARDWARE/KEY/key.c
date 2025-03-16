#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#define ROW_NUM 4
#define COL_NUM 4

// ����PB3-PB6
GPIO_TypeDef* ROW_PORT[ROW_NUM] = {GPIOB, GPIOB, GPIOB, GPIOB};
const uint16_t ROW_PIN[ROW_NUM] = {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6};
// ����PB7-PB10
GPIO_TypeDef* COL_PORT[COL_NUM] = {GPIOB, GPIOB, GPIOB, GPIOB};
const uint16_t COL_PIN[COL_NUM] = {GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  // ����P3-P6Ϊ��������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	// ����P7-P10Ϊ�������	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

}
uint8_t MatrixKeyScan(void)
{
		uint8_t keyValue[4][4] = {{16, 15, 14, 13},
		{12, 11, 10, 9},
		{8, 7, 6, 5},
		{4, 3, 2, 1}}; // �������ɨ����
		int i, j;

		// ������ߵ�ƽ
		for(i = 0; i < COL_NUM; i++) {
			GPIO_SetBits(COL_PORT[i], COL_PIN[i]);
		}

		// ����е�ƽ״̬
		for(i = 0; i < ROW_NUM; i++) {
			// ��������͵�ƽ
			GPIO_ResetBits(ROW_PORT[i], ROW_PIN[i]);
			delay_ms(10); // ��ʱȥ��

			for(j = 0; j < COL_NUM; j++) {
				if(GPIO_ReadInputDataBit(COL_PORT[j], COL_PIN[j]) == Bit_RESET) {
					while (GPIO_ReadInputDataBit(COL_PORT[j], COL_PIN[j]) == Bit_RESET);
					delay_ms(10); // ��ʱȥ��
					GPIO_SetBits(ROW_PORT[i], ROW_PIN[i]); // ������ߵ�ƽ
					return keyValue[i][j];
				}
			}

			GPIO_SetBits(ROW_PORT[i], ROW_PIN[i]); // ������ߵ�ƽ
		}

		return 0;

}




