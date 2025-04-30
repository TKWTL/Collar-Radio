/*系统数据更新文件
 *负责更新系统状态
 *并且在需要的时候触发弹窗
 */
/*所用引脚的宏定义在mian.h文件中，由CubeMX自动生成
 *
 *
 */
#include "system_statistic.h"


ChargeState_t eChargeState = DISCHARGING;//电池充放电模式

/*电池充放电模式更新函数
 *根据TP4057 CHRG与STDBY两个引脚的状态来判断电池充放电模式
 */
void UpdateChargeState(void){
    if(LL_GPIO_IsInputPinSet(CHRG_GPIO_Port, CHRG_Pin) == RESET){
        if(eChargeState != CHARGING) Notification_Send(1);//触发充电中弹窗
        eChargeState = CHARGING;
    }
    else if(LL_GPIO_IsInputPinSet(STDBY_GPIO_Port, STDBY_Pin) == RESET){
        if(eChargeState != CHARGED) Notification_Send(2);//触发充满弹窗
        eChargeState = CHARGED;
    }
    else{
        eChargeState = DISCHARGING;
    }
}

//ADC转换通道枚举，需要与CubeMX中的已使能通道次序对应
typedef enum{
    CH_VBAT,
    CH_ICHARGE,
    CH_TJ,
    CH_VCC,
    
    CH_TOTAL//代表总通道数，无需修改
}ADC_ChannelNO_t;

uint16_t adc_buffer[CH_TOTAL];//存放ADC转换结果的数组
uint32_t adc_accumulator[CH_TOTAL];//累加ADC转换结果的数组，用于均值滤波

float VCC = 3.3f;           //VCC电压值
float Tjunction = 15.0f;    //MCU结温，单位摄氏度    
float Icharge = 0.0f;       //充电电流，单位毫安
float Vbattery = 4.2f;      //电池电压

#define OVERSAMPLE_RATE     4   //均值滤波平均次数
#define SAMPLE_PERIOD       400//采样周期，单位ms

/*
 *采样ADC值并加以滤波
 */
void ADC_SampleandFilter(void){
    static uint8_t i = 0;
    static uint8_t lowpower_motificated = 0;
    uint8_t j;
    
    switch(i++){
        case OVERSAMPLE_RATE://一轮采样完成，滤波并更新
            VCC = 3.00f* *VREFINT_CAL_ADDR/ adc_accumulator[CH_VCC];
        
            Tjunction = (VCC* adc_accumulator[CH_TJ]/ 4095- 0.76f)* 400.0f+ 25;
        
            Icharge = VCC* 218.56f* adc_accumulator[CH_ICHARGE]/ 4095- 9.164f;
            if(Icharge < 10.0f) Icharge = 0.0f;
            //更新电池电量，但是刚刚唤醒时不更新
            if(Unstable_of_Wake == 0) Vbattery = adc_accumulator[CH_VBAT]* VCC/ 2048;
        
            /*电量策略
             *不会放电到低于2.5V以致电池保护板停止输出，RTC停止运行
             *3.1V下无论任何界面强制弹窗电量过低消息
             *15s后即强制进入休眠状态
             */
            if(Vbattery < 3.1f && lowpower_motificated == 0){
                lowpower_motificated = 1;
                Notification_Send(0);//触发低电量弹窗
                RadioPowerMode = RADIO_OFF;
            }
            if(Vbattery > 3.4f) lowpower_motificated = 0;
            //更新电池电量格数，并实现回差
            switch(Battery_level){
                case 4:
                    if(Vbattery < 3.85f) Battery_level = 3;
                    break;
                case 3:
                    if(Vbattery > 3.9f) Battery_level = 4;
                    if(Vbattery < 3.7f) Battery_level = 2;
                    break;
                case 2:
                    if(Vbattery > 3.75f) Battery_level = 3;
                    if(Vbattery < 3.6f) Battery_level = 1;
                    break;
                case 1:
                    if(Vbattery > 3.65f) Battery_level = 2;
                    if(Vbattery < 3.4f) Battery_level = 0;
                    break;
                case 0:
                    if(Vbattery > 3.45f) Battery_level = 1;
                    break;
                default:
                    Battery_level = 0;
            }
        
            for(j = 0; j < CH_TOTAL; j++) adc_accumulator[j] = 0;
            i = 0;
            osDelay(pdMS_TO_TICKS(SAMPLE_PERIOD/ (OVERSAMPLE_RATE+ 1)));
            break;
        
        default://采样周期
            HAL_ADC_Start_DMA(&hadc1, (uint32_t*)(adc_buffer), CH_TOTAL);
            osDelay(pdMS_TO_TICKS(SAMPLE_PERIOD/ (OVERSAMPLE_RATE+ 1)));//直接延时等待采样完成
            for(j = 0; j < CH_TOTAL; j++) adc_accumulator[j] += adc_buffer[j];
            break;
    }
}

uint8_t Battery_level = 3;//电池电量等级
