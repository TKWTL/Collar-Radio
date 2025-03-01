/*
 *用户应用文件夹下应用程序统一头文件
 */
#ifndef __APPLICATION_H__
#define __APPLICATION_H__

//系统
#include "main.h"

//RTOS接口
#include "cmsis_os.h"

//外设驱动库
#include "oled.h"       //OLED(SSD1312)驱动库
#include "bmp180.h"     //气压计驱动库
#include "rda5807m.h"   //收音机驱动库
#include "sc7a20.h"     //加速度计驱动库

//U8G2库
#include "u8g2.h"

//MiaoUI库
#include "ui.h"

//按键驱动库
#include "buttons.h"

extern osMessageQueueId_t Button_QueueHandle;

#endif