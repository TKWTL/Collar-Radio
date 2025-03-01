#ifndef __BMP180_H__
#define __BMP180_H__

#ifdef __cplusplus
extern "C" {
#endif
    
/*****************************用户配置区开始***********************************/
#include "main.h"                                                               //包含按键状态读取函数的库
/*****************************用户配置区结束***********************************/

#define BMP180_ADDRESS  0xEE                                                    //BMP180 8位从机地址

#define BMP180_ADCVALUE_BASE    0xF6
#define BMP180_CONTROL          0xF4
#define BMP180_SOFTRESET        0xE0
#define BMP180_ID               0xD0
#define BMP180_CALVALUE_BASE    0xAA
    
#define BMP180_AC1_OFFSET       0
#define BMP180_AC2_OFFSET       2
#define BMP180_AC3_OFFSET       4
#define BMP180_AC4_OFFSET       6
#define BMP180_AC5_OFFSET       8
#define BMP180_AC6_OFFSET       10
#define BMP180_B1_OFFSET        12
#define BMP180_B2_OFFSET        14
#define BMP180_MB_OFFSET        16
#define BMP180_MC_OFFSET        18
#define BMP180_MD_OFFSET        20

/*#define BMP180_LOAD\
            BMP180_StartConv(0x2E);\
            osDelay(pdMS_TO_TICKS(5));\
            BMP180_GetRawTemprature();\
            */
    
    
void BMP180_Init(void);
void BMP180_StartConv(uint8_t code);
void BMP180_GetTemprature(void);
void BMP180_GetPressure(void);
#ifdef __cplusplus
}
#endif

#endif