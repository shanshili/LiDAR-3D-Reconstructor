#ifndef __WIT_C_SDK_H
#define __WIT_C_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "REG.h"

#define WIT_HAL_OK      (0)     /**< There is no error */
#define WIT_HAL_INVAL   (-6)    /**< Invalid argument */
#define WIT_DATA_BUFF_SIZE  256

void WitSerialDataIn(uint8_t ucData);
typedef void (*RegUpdateCb)(uint32_t uiReg, uint32_t uiRegNum);
int32_t WitRegisterCallBack(RegUpdateCb update_func);
extern int16_t sReg[REGSIZE];

#ifdef __cplusplus
}
#endif

#endif /* __WIT_C_SDK_H */
