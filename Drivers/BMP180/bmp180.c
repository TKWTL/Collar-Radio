#include "bmp180.h"

uint8_t temp[4] = {0};
uint8_t bmp180_cal[22];                                                         //BMP180校准值寄存器，从0xAA ~ 0xBF
volatile int32_t Temprature_raw;
volatile int32_t Pressure_raw;
int32_t B5;

int BMP180_id = 0;                                                              //芯片ID，正常为0x55
float BMP180_AirTemprature;
int32_t BMP180_AirPressure;
float BMP180_AirPressure_kPa;
float BMP180_Altitude;

/*
 *初始化BMP180，将校准数据读取到内部寄存器中
 */
void BMP180_Init(void){
    temp[0] = BMP180_ID;
    I2C_Transmit(BMP180_ADDRESS, temp, 1);
    I2C_Receive(BMP180_ADDRESS, (uint8_t*)&BMP180_id, 1);
    if(BMP180_id != 0x55) return;
    
    temp[0] = BMP180_CALVALUE_BASE;
    I2C_Transmit(BMP180_ADDRESS, temp, 1);
    I2C_Receive(BMP180_ADDRESS, bmp180_cal, sizeof(bmp180_cal)/sizeof(uint8_t));
    //对字(Word)型数据进行翻转
    uint8_t tmp, i;
    for(i = 0;i < sizeof(bmp180_cal)/sizeof(uint8_t); i+=2){
        tmp = bmp180_cal[i+1];
        bmp180_cal[i+1] = bmp180_cal[i];
        bmp180_cal[i] = tmp;
    }
}

void BMP180_StartConv(uint8_t code){
    temp[0] = BMP180_CONTROL;
    temp[1] = code;
    I2C_Transmit(BMP180_ADDRESS, temp, 2);
}

void BMP180_GetTemprature(void){
    int32_t X1, X2;
    temp[0] = BMP180_ADCVALUE_BASE;
    I2C_Transmit(BMP180_ADDRESS, temp, 1);
    I2C_Receive(BMP180_ADDRESS, temp, 2);
    Temprature_raw = ((temp[0] << 8)+ temp[1])& 0x0000FFFFU;//UT
    X1 = ((Temprature_raw - *(uint16_t*)(bmp180_cal+ BMP180_AC6_OFFSET))* (*(uint16_t*)(bmp180_cal+ BMP180_AC5_OFFSET))) >> 15;//(UT - AC6)*AC5/32768
    X2 = ((int32_t)*(int16_t*)(bmp180_cal+ BMP180_MC_OFFSET)<<11)/ (X1+ *(int16_t*)(bmp180_cal+ BMP180_MD_OFFSET));
    B5 = X1+ X2;
    BMP180_AirTemprature = ((uint32_t)(B5+ 8)>>4)* 0.1f;
}

void BMP180_GetPressure(void){
    uint8_t oss = 3;
    int32_t B3, B6, X1, X2, X3;
    uint32_t B4, B7;
    temp[0] = BMP180_ADCVALUE_BASE;
    I2C_Transmit(BMP180_ADDRESS, temp, 1);
    I2C_Receive(BMP180_ADDRESS, temp, 3);
    Pressure_raw = ((temp[0] << 16)| (temp[1] << 8)| temp[2])& 0x00FFFFFFU;//UT
    Pressure_raw = Pressure_raw >> (8- oss);
    
    B6 = B5 - 4000;
    X1 = (*(int16_t*)(bmp180_cal+ BMP180_B2_OFFSET)* ((B6 * B6) >> 12)) >> 11;
    X2 = (*(int16_t*)(bmp180_cal+ BMP180_AC2_OFFSET) * B6) >> 11;
    X3 = X1 + X2;
    B3 = (((*(int16_t*)(bmp180_cal+ BMP180_AC1_OFFSET)* 4 + X3) << oss) + 2) >> 2;
    X1 = (*(int16_t*)(bmp180_cal+ BMP180_AC3_OFFSET)* B6) >> 13;
    X2 = (*(int16_t*)(bmp180_cal+ BMP180_B1_OFFSET)*((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2)+ 2) >> 2;
    B4 = (uint32_t)((*(uint16_t*)(bmp180_cal+ BMP180_AC4_OFFSET)* (uint32_t)(X3 + 32768)) >> 15);
    B7 = (uint32_t)((uint32_t)(Pressure_raw - B3)* (50000>>oss));
    if (B7 < 0x80000000U) BMP180_AirPressure = (int32_t)((B7 << 1) / B4);
    else BMP180_AirPressure = (int32_t)((B7/ B4) << 1);
    X1 = (BMP180_AirPressure >> 8) * (BMP180_AirPressure >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * BMP180_AirPressure) >> 16;
    BMP180_AirPressure = BMP180_AirPressure + ((X1 + X2 + 3791) >> 4);  
}
    