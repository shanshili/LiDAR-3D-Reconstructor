/************************************************
2CH PWM(TIM3):
	ARR:72,000,000/720/400=250hz
	OC2-PB5 ������ 2.604 2.554 3.065-128.309 127.985 128.919  7-8��/3-4�μ�
	OC3-PB0 ����� 0.077-154.111 3-7��/��
-116.180 -109.869 -105.914 -99.717 -96.268 -89.116 -85.820 -78.349 -77.080 -69.022 -66.940
6 4 6 3 7 

JY901S(USART3):
	USB-TTL                   STM32Core              		JY901s �����սǶ� ����x ���y ����z
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
#include "motor.h"
#include "OLED.h"
#include "display.h"

#define Smin 96 //76 123��
#define Smax 176//168 �ο�0��
#define Sstep 4//2*46
#define min  90
#define max  310 //ϵͳ��������������Ϊ242 ��������Ϊ370
#define step 10
#define set 3 //����ɨ���ܶ�
#define ANGLE_UPDATE	3
#define TIME_UPDATE   6
static void SensorDataUpdata(uint32_t uiReg, uint32_t uiRegNum);
static volatile char s_cDataUpdate = 0;
static uint16_t pwmval1=Smax; //PWM��1��ֵ
static	uint8_t dir1=1,dirM=1;	//PWM������־  //����Ǿ�̬�����������жϵ�Ӱ��


uint8_t u_rx_buf[400];
uint32_t TOF_data_length=400;		//DMA�������ݳ���

void TofM_Unpack_Data(uint8_t *u_rx_buf)
{
	uint8_t tofm_flag,tofm_id;
  tofm_flag = g_ntsm_frame0.UnpackData(u_rx_buf,TOF_data_length);		//��ȡ�����־ʹ
	tofm_id = g_ntsm_frame0.id;											//��ȡ��֡���ݵ�ģ��ID
	if(tofm_flag == 1)													//����ɹ�
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
				printf("���룺%f ",TOF_M[tofm_id].dis[i]);
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
u8 systemreset(u8 *dis,u8 flag,u8 *M,u8 *J,u8 *D,u8 tofm_id )
{
	OLED_Clear();
	//���reset
	pwmval1=Smax;	TIM_SetCompare2(TIM3,pwmval1);	
	//OLEDreset
	OLED_ShowString(48,2,dis,16);	display_Data(0000);
	OLED_Show_number(0,0,16,flag);
	//LIDAR JY901S reset
	do{}
	while(s_cDataUpdate == 0);
	OLED_ShowString(16,0,J,16); 
	do{}
	while(A_Distance == 0 );
	OLED_ShowString(24,0,D,16);
	do{}
	while(Monitor_Distance == 0 );
	OLED_ShowString(8,0,M,16); 		
	//flag = MRESET();
	flag = 1;
	OLED_Show_number(0,0,16,flag);
  return flag;
}


int main(void)
{		
	float fAngle[3];//JY901
//	int16_t fTime[6];//JY901
	int i; //JY901
	uint8_t tofm_id;
	tofm_id = g_ntsm_frame0.id;
	u8 sysflag = 0;
	u8 dis[] = "dis:";
	u8 M[] = "M";
	u8 J[] = "J";
	u8 D[] = "D";
//	u8 Mnum = 0;
//	u8 Mflag = 0;
	u8 round;

	delay_init();	 
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	uart2_init(921600);  //LIDAR
	uart3_init(115200);  //jy901s
	uart4_init(921600);  //Monitor LIDAR
	OLED_Init();	 
	Motor_Config(); //298���������ʼ��
	TIM3_PWM_Init(399,720);	 //����Ƶ��PWMƵ��=72,000,000/720/400=250hz
	WitRegisterCallBack(SensorDataUpdata);
	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 
  sysflag = systemreset(dis,sysflag,M,J,D,tofm_id);
	printf("X,X,Y,Y,Z,Z,D,D\r\n");
  while(sysflag)
	{	
		displayTEST_Data(pwmval1);
    delay_ms(100);//����仯���ۿɼ��ķ���
		/*TOF*/	
		printf("d,%d",A_Distance);
		printf(",s,%d\r\n",A_statue);
		/*jy901s*/
		for(i = 0; i < s_cDataUpdate; i++)
		{
			fAngle[i] = sReg[Roll+i] / 32768.0f * 180.0f;
		//	fTime[i] = sReg[YYMM+i];
		}
	/*
		if(s_cDataUpdate == TIME_UPDATE)
		{
			printf("time:%d��%d��%d��%dʱ%d��%d��\r\n", fTime[0],fTime[1],fTime[2],fTime[3],fTime[4],fTime[5]);
			s_cDataUpdate =0;
		}	
		*/
		if(s_cDataUpdate == ANGLE_UPDATE)
		{
			printf("X,%.3f,Y,%.3f,Z,%.3f,md,%d\r\n", fAngle[0], fAngle[1], fAngle[2],Monitor_Distance);
			//printf("{\'X\':%.3f,\'Y\':%.3f,\'Z\':%.3f,\'md\':%.0d}\r\n", fAngle[0], fAngle[1], fAngle[2],Monitor_Distance);
			s_cDataUpdate =0;
		}
		/*PWM*/ 
		if(dir1)pwmval1-=Sstep; //������PWM
		else pwmval1+=Sstep;//������PWM
		TIM_SetCompare2(TIM3,pwmval1);
		if(pwmval1 >= Smax) 
		{
			dir1=1;
		}
		if(pwmval1 == Smin) 
		{
			dir1=0;
		}
		/*Monitor*/
		if(dirM)
		{
			for(round = 0;round <= set;round++)
			{
			  FR();
			}
		}
		else 
		{
			for(round = 0;round <= set;round++)
			{
			  RF();
			}
		}
		if(Monitor_Distance >= max)
		{
		   dirM = 0;
		}
		if(Monitor_Distance <= min)
		{
		   dirM = 1;
		}
		display_Data(Monitor_Distance);		
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

