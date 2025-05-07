#ifndef __GRADIENTER_H__
#define __GRADIENTER_H__

#ifdef __cplusplus
extern "C" {
#endif
    
#include "application.h"

void Gradienter(ui_t *ui);
    
/***********************以下函数须在ui_conf.c中调用****************************/
//与MiaoUI的接口函数，用于注册水平仪的设置项
//创建加速度计模块所需的参数
void Create_Gradienter_Parameters (ui_t *ui);
//将加速度计模块的对象添加到菜单中
void Add_Gradienter_Items(ui_page_t *ParentPage);
    
#ifdef __cplusplus
}
#endif

#endif