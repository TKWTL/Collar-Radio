
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

/***********************以下函数须在ui_conf.c中调用****************************/
//创建显示屏模块所需的参数
void Create_Statistic_Parameters(ui_t *ui);
//将显示屏模块的对象添加到菜单中
void Add_Statistic_Items(ui_page_t *ParentPage);

#ifdef __cplusplus
}
#endif

#endif
