#ifndef __SYSTEM_OPERATION_H__
#define __SYSTEM_OPERATION_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "algorithm.h"
    
extern int AutoSleepEnable;//自动休眠设置，默认打开
extern int Sleep_time;//无操作休眠时间，单位秒
    
extern int WakePendingTime;//表示系统唤醒后的不响应时间
extern uint8_t Unstable_of_Wake;//表示系统由于唤醒产生的不稳定状态
    
//系统复位函数
void Manual_Reset(ui_t *ui);
//供外界调用以重置休眠倒计时的函数
void UserOperationDetected(void);
//循环执行的用于休眠的函数
void SystemSleep(void);
   
/***********************以下函数须在ui_conf.c中调用****************************/
//创建系统设置模块所需的参数
void Create_SystemCtrl_Parameters(ui_t *ui);
//将系统设置模块的对象添加到菜单中
void Add_SystemCtrl_Items(ui_page_t *ParentPage);

#ifdef __cplusplus
}
#endif

#endif
 