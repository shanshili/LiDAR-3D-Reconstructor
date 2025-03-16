#include "stm32f10x.h"
#include "OLED.h"
#include "OLEDFONT.h" 
 
 
/***********************Delay****************************************/
void OLED_Delay_50ms(unsigned int Del_50ms)
{
	unsigned int m;
	for(;Del_50ms>0;Del_50ms--)
	for(m=6245;m>0;m--);
}

void OLED_Delay_1ms(unsigned int Del_1ms)
{
	unsigned char j;
	while(Del_1ms--){	
		for(j=0;j<123;j++);
	}
}

  
/**********************************************/
//__________________________________________________________________________

/**********************************************
//IIC Start
**********************************************/
void OLED_IIC_Start()
{

	OLED_SCLK_Set() ;
	OLED_SDIN_Set();
	OLED_SDIN_Clr();
	OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void OLED_IIC_Stop()
{
	
  OLED_SCLK_Clr();
	OLED_SDIN_Clr();
	OLED_SDIN_Set();
	OLED_SCLK_Set() ;
}

void OLED_IIC_Wait_Ack()
{
 
	OLED_SCLK_Set() ;
	OLED_SCLK_Clr();
	
}
/**********************************************
// IIC Write byte
**********************************************/

void OLED_Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	unsigned char m;;
	OLED_SCLK_Clr();
	for(i=0;i<8;i++)		
	{
			OLED_SCLK_Clr();
			m=IIC_Byte&0x80;
			if(m==0x80){
				OLED_SDIN_Set();
			}
			else OLED_SDIN_Clr();
			IIC_Byte<<=1;
			OLED_SCLK_Set();
			OLED_SCLK_Clr();
	}

	  OLED_SCLK_Clr();
		OLED_SCLK_Set();
}

//__________________________________________________________________________

/**********************************************
// IIC Write Command
**********************************************/
void OLED_Write_IIC_Command(unsigned char IIC_Command)
{
   OLED_IIC_Start();
   OLED_Write_IIC_Byte(0x78);     //Slave address,SA0=0
	 OLED_IIC_Wait_Ack();	
   OLED_Write_IIC_Byte(0x00);			//write command
	 OLED_IIC_Wait_Ack();	
   OLED_Write_IIC_Byte(IIC_Command); 
	 OLED_IIC_Wait_Ack();	
   OLED_IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void OLED_Write_IIC_Data(unsigned char IIC_Data)
{
   OLED_IIC_Start();
   OLED_Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	 OLED_IIC_Wait_Ack();	
   OLED_Write_IIC_Byte(0x40);			//write data
	 OLED_IIC_Wait_Ack();	
   OLED_Write_IIC_Byte(IIC_Data);
	 OLED_IIC_Wait_Ack();	
   OLED_IIC_Stop();
}

//__________________________________________________________________________

/**********************************************
// OLED_WR_Byte
**********************************************/

void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd) {
    OLED_Write_IIC_Data(dat);
  }
	else{
   OLED_Write_IIC_Command(dat);	
	}
}
/**********************************************
// 坐标设置
**********************************************/

void OLED_Set_Pos(unsigned char x, unsigned char y){
 	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}
/**********************************************
// 开启OLED显示
**********************************************/
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
/**********************************************
// 关闭OLED显示
**********************************************/     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		


//__________________________________________________________________________

/**********************************************
//初始化SSD1306
**********************************************/				    
void OLED_Init(void)
{ 	
 	GPIO_InitTypeDef  GPIO_InitStructure;	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能A端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	  //初始化GPIOD3,6
 	GPIO_SetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);	
	OLED_Delay_50ms(16);
	
	// 正常0.91寸
	OLED_WR_Byte(0xAE,OLED_CMD);//--display off
	 
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	
	OLED_WR_Byte(0xc8,OLED_CMD); // contract control
	
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap 
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x1F,OLED_CMD);//--1/32 duty
	 
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0xf0,OLED_CMD);//
 
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0x22,OLED_CMD);//正常0.91寸
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x02,OLED_CMD);//正常0.91寸
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x49,OLED_CMD);//正常0.91寸
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}  



//__________________________________________________________________________





/**********************************************
// 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
**********************************************/  	  
void OLED_Clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
	} //更新显示
}
 

/**********************************************
// //更新显示
**********************************************/ 
void OLED_On(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA); 
	} //更新显示
}

/**********************************************
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~3
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
**********************************************/ 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
		unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
		}
		else {	
			OLED_Set_Pos(x,y);
			for(i=0;i<6;i++)
			OLED_WR_Byte(F6x8[c][i],OLED_DATA);
				
		}
}

/**********************************************
//显示一个字符号串 x是行
**********************************************/ 
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
//显示数字
void  OLED_Show_number(u8 x,u8 y,u8 Char_Size,u8 no ){
	//u8 t,adder=0;
	OLED_Set_Pos(x,y);
	OLED_ShowChar(x,y,(char)(no+'0'),Char_Size);
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
/*
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1)
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WR_Byte(BMP1[j++],OLED_DATA);	    	
	    }
	}
} 
*/




