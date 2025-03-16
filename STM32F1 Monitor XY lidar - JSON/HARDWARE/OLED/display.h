#include "stm32f10x.h"
#ifndef __DISPLAY_H
#define __DISPLAY_H

extern u8 data_state;

void display_init(void);
void display_Data(u32 x );
void display_clear(void);
void display_massage(void);
                                                                                                            
#endif

