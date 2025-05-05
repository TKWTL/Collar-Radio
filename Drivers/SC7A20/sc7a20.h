#ifndef __SC7A20_H__
#define __SC7A20_H__

#ifdef __cplusplus
extern "C" {
#endif
    
/*****************************用户配置区开始***********************************/
#include "main.h"                                                               //包含库
/*****************************用户配置区结束***********************************/
 
/* Defines ------------------------------------------------------------------*/
/***Before using the driver, configure it according to the actual cable connection******/
/**The SDO pin of SC7A20 is connected to ground:                     0****************/
/**The SDO pin of SC7A20 is connected to power supply or left float: 1****************/
#define SC7A20_SDO_VDD_GND            1
/*****************************************/
 
/***Before using the driver, configure the driver based on the actual IIC***/
/**The IIC interface address type of the SC7A20 is 7bits: 0****/
/**The IIC interface address type of the SC7A20 is 8bits: 1****/
#define SC7A20_IIC_7BITS_8BITS        1
/*****************************************/
 
#if SC7A20_SDO_VDD_GND == 0
#define SC7A20_IIC_7BITS_ADDR        0x18
#define SC7A20_IIC_8BITS_ADDR        0x30
#else
#define SC7A20_IIC_7BITS_ADDR        0x19
#define SC7A20_IIC_8BITS_ADDR        0x32
#endif
 
#if SC7A20_IIC_7BITS_8BITS == 0
#define SC7A20_IIC_ADDRESS       SC7A20_IIC_7BITS_ADDR
#else
#define SC7A20_IIC_ADDRESS       SC7A20_IIC_8BITS_ADDR
#endif
 
 
 
#define SC7A20_CHIP_ID_ADDRESS    (unsigned char)0x0F
#define SC7A20_CHIP_ID_VALUE      (unsigned char)0x11

#define SC7A20_CTRL_REG1      (unsigned char)0x20
#define SC7A20_CTRL_REG2      (unsigned char)0x21
#define SC7A20_CTRL_REG3      (unsigned char)0x22
#define SC7A20_CTRL_REG4      (unsigned char)0x23 
#define SC7A20_CTRL_REG5      (unsigned char)0x24
#define SC7A20_CTRL_REG6      (unsigned char)0x25

#define SC7A20_STATUS_REG     (unsigned char)0x27

#define SC7A20_OUT_X_L        (unsigned char)0x28
#define SC7A20_OUT_X_H        (unsigned char)0x29
#define SC7A20_OUT_Y_L        (unsigned char)0x2A
#define SC7A20_OUT_Y_H        (unsigned char)0x2B
#define SC7A20_OUT_Z_L        (unsigned char)0x2C
#define SC7A20_OUT_Z_H        (unsigned char)0x2D

#define SC7A20_FIFO_CTRL_REG  (unsigned char)0x2E
#define SC7A20_FIFO_SRC_REG   (unsigned char)0x2F

#define SC7A20_AOI1_CFG    	  (unsigned char)0x30
#define SC7A20_AOI1_SRC       (unsigned char)0x31
#define SC7A20_AOI1_THS    	  (unsigned char)0x32
#define SC7A20_AOI1_DURATION  (unsigned char)0x33

#define SC7A20_AOI2_CFG    	  (unsigned char)0x34
#define SC7A20_AOI2_SRC       (unsigned char)0x35
#define SC7A20_AOI2_THS    	  (unsigned char)0x36
#define SC7A20_AOI2_DURATION  (unsigned char)0x37
#define SC7A20_CLICK_CFG   	  (unsigned char)0x38
#define SC7A20_CLICK_SRC   	  (unsigned char)0x39
#define SC7A20_CLICK_THS   	  (unsigned char)0x3A
#define SC7A20_TIME_LIMIT     (unsigned char)0x3B
#define SC7A20_TIME_LATENCY   (unsigned char)0x3C
#define SC7A20_TIME_WINDOW    (unsigned char)0x3D
#define SC7A20_ACT_THS        (unsigned char)0x3E
#define SC7A20_ACT_DURATION   (unsigned char)0x3F
	
/*连续读取数据时的数据寄存器地址*/
#define SC7A20_DATA_OUT       (unsigned char)(SC7A20_OUT_X_L|0x80)

/**********特殊功能寄存器**********/
/*非原厂技术人员请勿修改*/
#define SC7A20_MTP_ENABLE    	             0x00
#define SC7A20_MTP_CFG    	  (unsigned char)0x1E
#define SC7A20_MTP_VALUE   	  (unsigned char)0x05
#define SC7A20_SDOI2C_PU_CFG  (unsigned char)0x57
#define SC7A20_SDO_PU_MSK     (unsigned char)0x08
#define SC7A20_I2C_PU_MSK     (unsigned char)0x04
#define SC7A20_HR_ENABLE      (unsigned char)0X08
#define SC7A20_BOOT_ENABLE    (unsigned char)0X80   
/*非原厂技术人员请勿修改*/


/***************数据更新速率**加速度计使能**********/
#define SC7A20_ODR_POWER_DOWN (unsigned char)0x07
#define SC7A20_ODR_1HZ        (unsigned char)0x17
#define SC7A20_ODR_10HZ       (unsigned char)0x27
#define SC7A20_ODR_25HZ       (unsigned char)0x37
#define SC7A20_ODR_50HZ       (unsigned char)0x47
#define SC7A20_ODR_100HZ      (unsigned char)0x57
#define SC7A20_ODR_200HZ      (unsigned char)0x67
#define SC7A20_ODR_400HZ      (unsigned char)0x77
#define SC7A20_ODR_1600HZ     (unsigned char)0x87
#define SC7A20_ODR_1250HZ     (unsigned char)0x97
#define SC7A20_ODR_5000HZ     (unsigned char)0x9F
    
#define SC7A20_LP_ODR_1HZ        (unsigned char)0x1F
#define SC7A20_LP_ODR_10HZ       (unsigned char)0x2F
#define SC7A20_LP_ODR_25HZ       (unsigned char)0x3F
#define SC7A20_LP_ODR_50HZ       (unsigned char)0x4F
#define SC7A20_LP_ODR_100HZ      (unsigned char)0x5F
#define SC7A20_LP_ODR_200HZ      (unsigned char)0x6F
#define SC7A20_LP_ODR_400HZ      (unsigned char)0x7F
/***************数据更新速率**加速度计使能**********/

#define SC7A20_CTRL4_BLE        0x40U
/***************传感器量程设置**********************/
#define SC7A20_FS_2G          (unsigned char)0x00		
#define SC7A20_FS_4G          (unsigned char)0x10
#define SC7A20_FS_8G          (unsigned char)0x20	
#define SC7A20_FS_16G         (unsigned char)0x30

#define SC7A20_CTRL4_HR         0x08U

/***取值在0-127之间，此处仅举例****/
#define SC7A20_INT_THS_5PERCENT   (unsigned char)0x06
#define SC7A20_INT_THS_8PERCENT   (unsigned char)0x09    
#define SC7A20_INT_THS_10PERCENT  (unsigned char)0x0C
#define SC7A20_INT_THS_20PERCENT  (unsigned char)0x18
#define SC7A20_INT_THS_40PERCENT  (unsigned char)0x30
#define SC7A20_INT_THS_80PERCENT  (unsigned char)0x60


/***取值在0-127之间，此处仅举例 乘以ODR单位时间****/
#define SC7A20_INT_DURATION_2CLK  (unsigned char)0x02
#define SC7A20_INT_DURATION_5CLK  (unsigned char)0x05
#define SC7A20_INT_DURATION_10CLK (unsigned char)0x0A

/***中断有效时的电平设置，高电平相当于上升沿，低电平相当于下降沿****/
#define SC7A20_INT_ACTIVE_LOWER_LEVEL 0x02 //0x02:中断时INT1脚输出 低电平
#define SC7A20_INT_ACTIVE_HIGH_LEVEL  0x00 //0x00:中断时INT1脚输出 高电平

/***中断有效时的电平设置，高电平相当于上升沿，低电平相当于下降沿****/
#define SC7A20_INT_AOI1_INT1          0x40 //AOI1 TO INT1
#define SC7A20_INT_AOI2_INT1          0x20 //AOI2 TO INT1

typedef struct{
    uint16_t X_RawData;
    uint16_t Y_RawData;
    uint16_t Z_RawData;
} SC7A20_Handle_t;

//初始化SC7A20，也用于上电
void SC7A20_Init(void);
//将SC7A20置于低功耗状态
void SC7A20_SetLowPower(void);
uint8_t SC7A20_id_get(void);
//从传感器加载加速度值
void SC7A20_Outdata_Load(void);

float SC7A20_readaccel_x(void);
float SC7A20_readaccel_y(void);
float SC7A20_readaccel_z(void);
    
#ifdef __cplusplus
}
#endif

#endif