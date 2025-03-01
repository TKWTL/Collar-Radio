
#include "application.h"

#define VBAT_CH     0
#define ICHARGE_CH  1
#define TJ_CH       2
#define VCC_CH      3

#define TOL_CH      4

#define OVERSAMP_RATE   4

uint16_t adc_buffer[TOL_CH* OVERSAMP_RATE];//存放ADC转换结果的数组

float VCC = 3.3f;
float Tjunction = 15.0f;
float Icharge = 0.0f;
float Vbattery = 3.6f;

void ADC_Task(void *argument)
{
    uint8_t i, j;
    for(;;)
    {
        for(i = 0; i < OVERSAMP_RATE; i++){
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(adc_buffer+ TOL_CH* i), TOL_CH);
            osDelay(pdMS_TO_TICKS(200));
        }
        for(i = 1; i < OVERSAMP_RATE; i++)
            for(j = 0; j < TOL_CH; j++)
                adc_buffer[j] += adc_buffer[i* OVERSAMP_RATE+ j];//累加各通道值
        
        VCC = 3.00f* *VREFINT_CAL_ADDR/ adc_buffer[VCC_CH];
        Tjunction = (VCC* adc_buffer[TJ_CH]/ 4095- 0.76f)* 400.0f+ 25;
        Icharge = VCC* 218.56f* adc_buffer[ICHARGE_CH]/ 4095- 9.164f;
        if(Icharge < 10.0f) Icharge = 0.0f;
        Vbattery = adc_buffer[VBAT_CH]* VCC/ 2048;
        osDelay(pdMS_TO_TICKS(200));
    }
}