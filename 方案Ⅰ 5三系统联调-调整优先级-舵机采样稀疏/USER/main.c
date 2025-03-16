/************************************************
2CH PWM(TIM3):
	ARR:72,000,000/720/400=250hz
	OC2-PB5 俯仰角 2.604 2.554 3.065-128.309 127.985 128.919  7-8°/3-4次加
	OC3-PB0 航向角 0.077-154.111 3-7°/次
-116.180 -109.869 -105.914 -99.717 -96.268 -89.116 -85.820 -78.349 -77.080 -69.022 -66.940
6 4 6 3 7 

JY901S(USART3):
	USB-TTL                   STM32Core              		JY901s 仅接收角度 俯仰x 横滚y 航向z
	VCC          -----           VCC        ----        	VCC
	TX           -----           RX1  (PA10)   
	RX           -----           TX1  (PA9)
	GND          -----           GND    ----       			 GND
															 RX2  (PB11)  ----        TX
															 TX2  (PB10)  ----        RX
LIDAR(USART2):
	TX PA2 
	RX PA3 
************************************************/

#include "timer.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"
#include "wit_c_sdk.h"
#include "REG.h"
#include "nlink_tofsensem_frame0.h"
 
#define ANGLE_UPDATE	3
#define TIME_UPDATE   6
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static volatile char s_cDataUpdate = 0;
static uint16_t pwmval1=50,pwmval2=50; //PWM比较值
static	uint8_t dir1=1,dir2=1;	//PWM往返标志  //如果非静态变量可能受中断等影响

uint8_t u_rx_buf[400];
uint32_t TOF_data_length=400;		//DMA接收数据长度

void TofM_Unpack_Data(uint8_t *u_rx_buf)
{
	uint8_t tofm_flag,tofm_id;
  tofm_flag = g_ntsm_frame0.UnpackData(u_rx_buf,TOF_data_length);		//获取解包标志使
	tofm_id = g_ntsm_frame0.id;											//获取该帧数据的模块ID
	if(tofm_flag == 1)													//解包成功
	{
		TOF_M[tofm_id].ID = tofm_id;
		TOF_M[tofm_id].pixel_count = g_ntsm_frame0.pixel_count;
		for(int i=0;i<TOF_M[tofm_id].pixel_count;i++)
		{
			TOF_M[tofm_id].dis[i] = g_ntsm_frame0.pixels[i].dis;
			TOF_M[tofm_id].status[i] = g_ntsm_frame0.pixels[i].dis_status;
			TOF_M[tofm_id].signal_strength[i] = g_ntsm_frame0.pixels[i].signal_strength;
		}
		/*
		for(int i=0;i<64;i++)
		{
				printf("距离：%f ",TOF_M[tofm_id].dis[i]);
		}
		*/
	}
}

uint8_t verifyCheckSum(uint8_t *data, int32_t length)
{ 
	uint8_t sum = 0;
	for(int32_t i=0;i<length-1;++i)
	{
		sum += data[i];
	}
	return sum == data[length-1];
	
}

unsigned char hex_to_char(unsigned char hex)
{
    if((hex>=0x30)&&(hex<=0x39))
        hex = hex-0x30;
    else if((hex>=0x41)&&(hex<=0x5A)) // capital
        hex = 'A' + (hex - 0x41);
    else if((hex>=0x61)&&(hex<=0x7A)) // little case
        hex = 'a' + (hex - 0x61);
    else 
        hex = 0xff;
    return hex;
}
 int main(void)
 {		
	float fAngle[3];//JY901
//	int16_t fTime[6];//JY901
	int i; //JY901
	uint8_t tofm_id;
	tofm_id = g_ntsm_frame0.id;
	 
	uart_init(115200);	 //串口初始化为115200
	uart2_init(921600);  //LIDAR
	uart3_init(115200);  //jy901s
	TIM3_PWM_Init(399,720);	 //不分频。PWM频率=72,000,000/720/400=250hz
	WitRegisterCallBack(SensorDataUpdata);
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_ms(2400);

  while(1)
	{	
			int j=0; //LIDAR标志
			delay_ms(2800);
			if(dir1)pwmval1+=25; //俯仰角PWM
			else pwmval1-=25;//俯仰角PWM
			
			if(s_cDataUpdate != 0 ) //jy901s
			{
				for(i = 0; i < s_cDataUpdate; i++)
				{
					fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
				//	fTime[i] = sReg[YYMM+i];
				}
			/*
				if(s_cDataUpdate == TIME_UPDATE)
				{
					printf("time:%d年%d月%d日%d时%d分%d秒\r\n", fTime[0],fTime[1],fTime[2],fTime[3],fTime[4],fTime[5]);
					s_cDataUpdate =0;
				}	
				*/
				if(s_cDataUpdate == ANGLE_UPDATE)
				{
					printf("{\'X\':%.3f,\'Y\':%.3f,\'Z\':%.3f}\r\n", fAngle[0], fAngle[1], fAngle[2]);
					s_cDataUpdate =0;
				}
			}
		 if(TOF_M[tofm_id].dis[63] + TOF_M[tofm_id].signal_strength[63] != 0) //雷达启动等候数据
		 {		
			for(j=0;j<64;j++)//lidar测距
			{ 
				if(TOF_M[tofm_id].status[j] != 255)
				{
					printf("{\'n\':%d",(j+1));
					printf(",\'d\':%.0f",TOF_M[tofm_id].dis[j]);
					printf(",\'s\':%d}\r\n",TOF_M[tofm_id].signal_strength[j]);
				}
			}		
			if(pwmval1>150) //航向角PWM 往
			{
				dir1=0;
				if(dir2)pwmval2+=25;
				else pwmval2-=25;
				if(pwmval2>150)dir2=0;
				if(pwmval2==50)dir2=1;	
				TIM_SetCompare3(TIM3,pwmval2);
			}
			if(pwmval1==50) //航向角PWM 返		
			{
				dir1=1;
				if(dir2)pwmval2+=25;
				else pwmval2-=25;
				if(pwmval2>150)dir2=0;
				if(pwmval2==50)dir2=1;	
				TIM_SetCompare3(TIM3,pwmval2);
			}		
			TIM_SetCompare2(TIM3,pwmval1);	
		}
  }	
}
 
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum)
{
		switch(uiReg)
		{
			case Roll:
					s_cDataUpdate = ANGLE_UPDATE;
			break;
			case YYMM:
					s_cDataUpdate = TIME_UPDATE;
			break;
		}
}

