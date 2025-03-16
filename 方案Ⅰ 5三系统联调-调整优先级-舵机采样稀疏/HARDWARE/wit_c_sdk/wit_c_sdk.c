#include "wit_c_sdk.h"
#include "REG.h"
#include "usart.h"	

static RegUpdateCb p_WitRegUpdateCbFunc = NULL;
static uint8_t inputdata[11];
static uint32_t inputdatasum = 0;
int16_t sReg[REGSIZE];//���䵽������

static uint8_t __CaliSum(uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint8_t ucCheck = 0;
    for(i=0; i<len; i++) ucCheck += *(data + i);
    return ucCheck;
}

void WitSerialDataIn(uint8_t ucData)
{
    uint16_t usData[6];//�����ֽ�����
    uint8_t ucSum;//У���
	  inputdata[inputdatasum] = ucData;
	  if(inputdata[0] == 0x55)//��Э��ͷ��ʼ����
		{      
			inputdatasum++;	
			if(inputdatasum == 11) //��ȡ��һ֡Э��
			{
				ucSum = __CaliSum(inputdata, 10);  //�ж�У���
				if(ucSum == inputdata[10])         //У�����ȷ
				{
					if(inputdata[1] == 0x53)
					{
							usData[0] = (short)((short)inputdata[3] << 8) | inputdata[2];//�ֱ��ȡ����
							usData[1] = (short)((short)inputdata[5] << 8) | inputdata[4];
							usData[2] = (short)((short)inputdata[7] << 8) | inputdata[6];
							usData[3] = (short)((short)inputdata[9] << 8) | inputdata[8];
							memcpy(&sReg[Roll], usData, 8);//!!!һ��usDataռ2���ֽ�
							p_WitRegUpdateCbFunc(Roll, 3); //��������ַ�� ��ȡ�Ĵ�������
					}
					/*
					if(inputdata[1] == 0x50)
					{
						  usData[0] = (int)inputdata[2];
						  usData[1] = (int)inputdata[3];
						  usData[2] = (int)inputdata[4];
						  usData[3] = (int)inputdata[5];
						  usData[4] = (int)inputdata[6];
						  usData[5] = (int)inputdata[7];
				 		  memcpy(&sReg[YYMM], usData, 12);
							p_WitRegUpdateCbFunc(YYMM, 3); //��������ַ�� ��ȡ�Ĵ�������
				  } 
					*/
				}
			}
			if(inputdatasum > 10)
			{
				 inputdatasum = 0;
			}
		}
}


int32_t WitRegisterCallBack(RegUpdateCb update_func)
{
    if(!update_func)return WIT_HAL_INVAL;
    p_WitRegUpdateCbFunc = update_func;
    return WIT_HAL_OK;
}

