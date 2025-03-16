#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#define ROW_NUM 4
#define COL_NUM 4

// 行线PB3-PB6
GPIO_TypeDef* ROW_PORT[ROW_NUM] = {GPIOB, GPIOB, GPIOB, GPIOB};
const uint16_t ROW_PIN[ROW_NUM] = {GPIO_Pin_3, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6};
// 列线PB7-PB10
GPIO_TypeDef* COL_PORT[COL_NUM] = {GPIOB, GPIOB, GPIOB, GPIOB};
const uint16_t COL_PIN[COL_NUM] = {GPIO_Pin_7, GPIO_Pin_8, GPIO_Pin_9, GPIO_Pin_10};
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK精英STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
	GPIO_InitTypeDef 	GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
  // 配置P3-P6为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	// 配置P7-P10为推挽输出	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &GPIO_InitStructure);

}
uint8_t MatrixKeyScan(void)
{
		uint8_t keyValue[4][4] = {{16, 15, 14, 13},
		{12, 11, 10, 9},
		{8, 7, 6, 5},
		{4, 3, 2, 1}}; // 矩阵键盘扫描码
		int i, j;

		// 列输出高电平
		for(i = 0; i < COL_NUM; i++) {
			GPIO_SetBits(COL_PORT[i], COL_PIN[i]);
		}

		// 检测行电平状态
		for(i = 0; i < ROW_NUM; i++) {
			// 行线输出低电平
			GPIO_ResetBits(ROW_PORT[i], ROW_PIN[i]);
			delay_ms(10); // 延时去抖

			for(j = 0; j < COL_NUM; j++) {
				if(GPIO_ReadInputDataBit(COL_PORT[j], COL_PIN[j]) == Bit_RESET) {
					while (GPIO_ReadInputDataBit(COL_PORT[j], COL_PIN[j]) == Bit_RESET);
					delay_ms(10); // 延时去抖
					GPIO_SetBits(ROW_PORT[i], ROW_PIN[i]); // 行输出高电平
					return keyValue[i][j];
				}
			}

			GPIO_SetBits(ROW_PORT[i], ROW_PIN[i]); // 行输出高电平
		}

		return 0;

}




