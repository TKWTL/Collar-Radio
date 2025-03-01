/*
 *系统休眠任务文件
 */
#include "application.h"

void SysShutdown_Task(void *argument){
    for(;;){
        osDelay(1);
    }
}
