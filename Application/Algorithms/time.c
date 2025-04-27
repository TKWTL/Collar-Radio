/*
 *重新封装了HAL库的时间相关格式
 *避免在默认的packed数据结构下M0+的非对齐访问出现Hardfault
 */
#include "time.h"

RTC_TimeTypeDef_Aligned sTime;//时间全局变量，值全部4字节对齐以避免M0+内核的Hardfault错误

RTC_TimeTypeDef RTC_Time;//时间全局变量，与HAL库的接口
RTC_DateTypeDef RTC_Date;

void RTC_UpdateTime(void){//从RTC更新时间
    HAL_RTC_GetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);
    sTime.Year = RTC_Date.Year;
    sTime.Month = RTC_Date.Month;
    sTime.Date = RTC_Date.Date;
    sTime.WeekDay = RTC_Date.WeekDay;
    sTime.Hours = RTC_Time.Hours;
    sTime.Minutes = RTC_Time.Minutes;
    sTime.Seconds = RTC_Time.Seconds;
}

void SetTime(ui_t *ui){//将时间更新到RTC
    RTC_Date.Year = sTime.Year;
    RTC_Date.Month = sTime.Month;
    RTC_Date.Date = sTime.Date;
    RTC_Date.WeekDay = sTime.WeekDay;
    HAL_RTC_SetDate(&hrtc, &RTC_Date, RTC_FORMAT_BIN);
    RTC_Time.Hours = sTime.Hours;
    RTC_Time.Minutes = sTime.Minutes;
    RTC_Time.Seconds = sTime.Seconds;
    HAL_RTC_SetTime(&hrtc, &RTC_Time, RTC_FORMAT_BIN);
}
