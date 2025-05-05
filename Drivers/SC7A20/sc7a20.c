#include "SC7A20.h"

/* Globals ------------------------------------------------------------------*/
static SC7A20_Handle_t SC7A20;

static void SC7A20_write_reg(uint8_t regAddr, uint8_t data){
    uint8_t temp[2] = {regAddr, data};
    I2C_Transmit(SC7A20_IIC_ADDRESS, temp, 2U);
}
 
static void SC7A20_read_reg(uint8_t regAddr, uint8_t *buf){	
    I2C_Transmit(SC7A20_IIC_ADDRESS, &regAddr, 1U);
    I2C_Receive(SC7A20_IIC_ADDRESS, buf, 1U);
}
 
static void SC7A20_multiRead_reg(uint8_t regAddr, uint8_t *buf, uint8_t len){
    I2C_Transmit(SC7A20_IIC_ADDRESS, &regAddr, 1U);
    I2C_Receive(SC7A20_IIC_ADDRESS, buf, len);
}

//初始化SC7A20，也用于上电
void SC7A20_Init(void){
    SC7A20_write_reg(SC7A20_CTRL_REG1, SC7A20_LP_ODR_10HZ);
    SC7A20_write_reg(SC7A20_CTRL_REG4, SC7A20_FS_2G| SC7A20_CTRL4_HR);
}

//将SC7A20置于低功耗状态
void SC7A20_SetLowPower(void){
    SC7A20_write_reg(SC7A20_CTRL_REG1, SC7A20_ODR_POWER_DOWN);
}
    

uint8_t SC7A20_id_get(void){
    uint8_t SC7A20_chip_id = 0;
    SC7A20_read_reg(SC7A20_CHIP_ID_ADDRESS, &SC7A20_chip_id);
    return SC7A20_chip_id;
}

//从传感器加载加速度值
void SC7A20_Outdata_Load(void){
    SC7A20_multiRead_reg(SC7A20_DATA_OUT, (uint8_t*)&SC7A20.X_RawData, 6);
}

//SC7A20 12位补码数据转换为国际单位制
float SC7A20_12bitComplement(uint16_t Data){
    int16_t Data_temp = Data >> 4;
    if(Data_temp >= 2048) Data_temp -= 4096;
    return 9.8f* 0.001f* Data_temp;
}

float SC7A20_readaccel_x(void){
    return SC7A20_12bitComplement(SC7A20.X_RawData);
}

float SC7A20_readaccel_y(void){
    return SC7A20_12bitComplement(SC7A20.Y_RawData);
}

float SC7A20_readaccel_z(void){
    return SC7A20_12bitComplement(SC7A20.Z_RawData);
}
