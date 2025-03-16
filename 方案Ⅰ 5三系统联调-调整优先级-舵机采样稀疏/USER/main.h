#ifndef __MAIN_H
#define __MAIN_H
#include <stdint.h>

void TofM_Unpack_Data(uint8_t *u_rx_buf);
uint8_t verifyCheckSum(uint8_t *data, int32_t length);
unsigned char hex_to_char(unsigned char hex);

/*TOFSenseM/MS�ͺŵĽ�����ݽṹ��*/

typedef struct {
	uint8_t pixel_count;		//8x8 or 4x4
	uint8_t ID;

	float dis[64];					//�����ľ�������
	uint8_t status[64];				//�������ź�״��ָ�j
	uint16_t signal_strength[64];	//�������ź�ǿ��
}Tofsense_M;
static Tofsense_M TOF_M[8];

#endif
