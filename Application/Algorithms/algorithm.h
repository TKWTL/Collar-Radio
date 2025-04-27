/*
 *用户应用文件夹下底层算法统一头文件
 */
#ifndef __ALGORITHM_H__
#define __ALGORITHM_H__

#ifdef __cplusplus
extern "C" {
#endif

//系统
#include "main.h"

//RTOS接口
#include "cmsis_os2.h"

//U8G2库
#include "u8g2.h"
    
//外设驱动库
#include "oled.h"       //OLED(SSD1312)驱动库
#include "bmp180.h"     //气压计驱动库
#include "rda5807m.h"   //收音机驱动库
#include "sc7a20.h"     //加速度计驱动库

//按键驱动库
#include "buttons.h"
    
//MiaoUI库
#include "ui.h"
#include "dispDriver.h"
#include "indevDriver.h"
    
//该层次各应用函数头文件
#include "time.h"
#include "system_operation.h"
#include "radio_operation.h"
#include "system_statistic.h"
#include "display_effects.h"

#include "application.h"

#ifdef __cplusplus
}
#endif

#endif
 