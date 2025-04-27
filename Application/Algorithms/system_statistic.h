
#ifndef __SYSTEM_STATISTIC_H__
#define __SYSTEM_STATISTIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "algorithm.h"

typedef enum{
    DISCHARGING,
    CHARGING,
    CHARGED
}ChargeState_t;

extern ChargeState_t eChargeState;//充电状态

void UpdateChargeState(void);

extern float VCC;           //VCC电压值
extern float Tjunction;     //MCU结温，单位摄氏度    
extern float Icharge;       //充电电流，单位毫安
extern float Vbattery;      //电池电压
void ADC_SampleandFilter(void);

#define BATTERY_LEVEL_MAX   4
extern uint8_t Battery_level;//电池电量的格数

#ifdef __cplusplus
}
#endif

#endif
