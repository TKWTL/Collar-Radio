/*
 *系统休眠任务文件
 */
#include "application.h"

void SysShutdown_Task(void *argument){
    for(;;){
        SystemSleep();
        osDelay(pdMS_TO_TICKS(10));
    }
}
