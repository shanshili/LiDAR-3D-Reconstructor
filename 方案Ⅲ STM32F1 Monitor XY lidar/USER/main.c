/**
PC2 PC3 PC4 PC5
IN1 IN2 IN3 IN4
01  03  02  04
红  黄  绿  蓝
A   B   A-  B-
31cm/0.4 = 77.5

PC6 PC7 PC8 PC9
IN1 IN2 IN3 IN4
01  03  02  04
A   B   A-  B-

———————————————————
0        》》     < 》》      <<<<<<<
V(12s) A     V    A     V     A
1
V      A     V    A     V     A
2
V      A     V    A     V     A
3
V      A     V    A     V     A
4
V      A     V    A     V     A
5
V      A     V    A     V     A
6零》》一   二》》三    四》》五
———————————————————

键盘 PB3-PB6   r1-r4
     PB7-PB10  c1-c4

LIDAR(USART2):
	TX PA2 
	RX PA3 
**/
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "motor.h"
#include "OLED.h" //修改路径
#include "display.h"
#include "key.h"
#include "nlink_tofsensem_frame0.h"

#define MesureYRound 500 //28:20mm
#define MesureXRound 250 //57:20mm
#define YRound 500 //28:20mm
#define Round 125//57:10mm  28:5mm
#define min  100
#define max  200
#define Xchange 3
#define Ychange 6
 
uint8_t u_rx_buf[400];
uint32_t TOF_data_length=400;		//DMA接收数据长度
uint8_t tofm_id;
void Mesure(void);

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
	 	u8 flag = 0;//,Mresetflag = 0
 
		Motor_Config(); //298电机驱动初始化
		delay_init();  //延时函数初始化
	  KEY_Init();
	 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		uart_init(115200);	 //串口初始化为115200
		uart2_init(921600);  //串口2初始化为921600
	 	//printf("sftart");
		//flag = MRESET();	
    flag = 1;	 
    uint8_t keys = 0;
		while(flag)
		{
			  int i = 0; //key标志
				keys = MatrixKeyScan();
			  //printf("scan");
			  if(keys !=0)
			  {
				   //printf("%d\r\n",keys);
				}
				
				if(keys == 3)//FR()
				{
						printf("%d\r\n",keys);
						for(i =0;i<=Round;i++)
					{
						 FR(); printf("1");
					}
					Motor_reset();
				}
				if(keys == 4)//RF()
				{
						printf("%d\r\n",keys);
						for(i =0;i<=Round;i++)
					{
						 RF();printf("0");
					}
					Motor_reset();
				}
				if(keys == 7)//YFR()
				{
						printf("%d\r\n",keys);
						for(i =0;i<=YRound;i++)
					{
						 YFR(); printf("1");
					}
					MotorY_reset();
				}
				if(keys == 8)//YRF()
				{
						printf("%d\r\n",keys);
						for(i =0;i<=YRound;i++)
					{
						 YRF();printf("0");
					}
					MotorY_reset();
				}
				if(keys == 2)//开始测量
				{
						//printf("%d\r\n",keys);
						Mesure();
				}
				
   }

}
 
void Mesure(void)
{
//		do{}
//		while(Mresetflag != 0);
	 tofm_id = g_ntsm_frame0.id;
	 int X = 0,Y = 0,j = 0,i = 0,r = 0;//LIDAR标志j 
	 u8 dirY = 0,dirX = 0;
	 //printf("开始测量");
	 //printf("X,x,y,y,d,d\r\n");
	while(1)
	{
		if(TOF_M[tofm_id].dis[36] + TOF_M[tofm_id].signal_strength[36] != 0) //雷达启动等候数据
	 {	
      printf("X,%d",X);
			printf(",Y,%d,d,d,z,z\r\n",Y);		 
			for(j=0;j<64;j++)//lidar测距
			{ 
				if(TOF_M[tofm_id].status[j] != 255)
				{
					printf("n,%d",(j+1));
					printf(",d,%.0f",TOF_M[tofm_id].dis[j]);
					printf(",z,%d",TOF_M[tofm_id].status[j]);
					printf(",s,%d\r\n",TOF_M[tofm_id].signal_strength[j]);
				}
			}
			if(dirY == 0) //前进
			{
					if(Y == Ychange) //Y change 换向flag更新 平移
					{ 
						 dirY = 1; 
						 if(dirX == 0)//FR
						 {
							 			if(X == Xchange)//直接折返
										{
												//dirX = 1 ;
												for(r = 0;r<Xchange;r++)
												{
													for(i =0;i<=(MesureXRound);i++)
													{
														 RF();									
													}
													Motor_reset();
												}
												//printf("<<<<<<<<<<<<<<<<<<<<<<<\r\n");	
												X = 0;
												dirX = 0;
										}
										else{//FR 更新位置
												for(i = 0;i<=MesureXRound;i++)
												{
													 FR();
												}
												Motor_reset();
												//printf(">>\r\n");
												X++;	
									  }									
						  }
						 /*
							else//直接折返
							{
										if(X == 0)//X change
										{
												dirX = 0 ;
										}
										else{//RF 更新位置
												for(i =0;i<=MesureXRound;i++)
												{
													 RF();			 
												}
									  }
										X--;
								for(i =0;i<=(MesureXRound*5);i++)
								{
									 RF();			 	
								}
	  						printf("<<<<<<<<<<<<<<<<<<<<<<<\r\n");	
                X = 0;
                dirX = 0; 								
							}
					    */
					}
					else{ //前进
							for(i =0;i<=MesureYRound;i++)//YFR
							{
								 YFR();			 
							}
							MotorY_reset();
							//printf("V\r\n"); 
							Y++;//更新位置
					}

			}
			else//后退
			{
					if(Y == 0)//Y change 换向flag更新 平移
					{ 
							dirY = 0 ;
						  if(dirX == 0)//FR
						 {
							 			if(X == Xchange)//直接折返
										{
											  //dirX = 1 ;
												for(r = 0;r<Xchange;r++)
												{
													for(i =0;i<=(MesureXRound);i++)
													{
														 RF();									
													}
													Motor_reset();
												}
												//printf("<<<<<<<<<<<<<<<<<<<<<<<\r\n");	
												X = 0;
												dirX = 0;
											 
										}
										else{//FR 更新位置
												for(i = 0;i<=MesureXRound;i++)
												{
													 FR();
												}
												Motor_reset();
												//printf(">>\r\n"); 
												X++;	
									  }
						  }
					}
					else{//后退
							for(i =0;i<=MesureYRound;i++)//YRF
							{
								 YRF();			 
							}
							MotorY_reset();
							//printf("A\r\n"); 
							Y--;//更新位置
				  }
			}
	 }
//	 else
//	 {
//	 				printf("36d,%.0f",TOF_M[tofm_id].dis[36]);
//		      printf(",z,%d",TOF_M[tofm_id].status[36]); 
//					printf(",s,%d\r\n",TOF_M[tofm_id].signal_strength[36]); 
//	 }
}
}
