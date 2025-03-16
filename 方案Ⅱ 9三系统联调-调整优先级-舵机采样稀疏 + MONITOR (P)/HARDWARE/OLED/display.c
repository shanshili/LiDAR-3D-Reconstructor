#include "display.h"
#include "delay.h"  
#include "OLED.h" 
#include "usart.h"	
#include "display.h"

u8 data_state;

void display_init(){
	OLED_Init();			//初始化OLED  
	OLED_Clear();	 
}

void display_clear(){
	OLED_Clear();			//初始化OLED  
}
 

void display_Data(u32 x )
{	
	u8 x0= x/1000%10;
	u8 x1= x/100%10;
	u8 x2= x/10%10;
	u8 x3= x%10;
 	 
	OLED_Show_number(80,2,16,x0);
	//if(~((x0 ==0) &&(x1 ==0)))
	OLED_Show_number(88,2,16,x1);
	//if(~((x0 ==0) &&(x1 ==0)&&(x2 ==0)))
	OLED_Show_number(96,2,16,x2);
	OLED_Show_number(104,2,16,x3); 
}

void displayTEST_Data(u16 x )
{	
	u8 x1= x/100%10;
	u8 x2= x/10%10;
	u8 x3= x%10;
 	 
	OLED_Show_number(2,2,16,x1);
	//if(~((x0 ==0) &&(x1 ==0)&&(x2 ==0)))
	OLED_Show_number(10,2,16,x2);
	OLED_Show_number(18,2,16,x3); 
}

