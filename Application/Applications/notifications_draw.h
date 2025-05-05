
#ifndef __NOTIFICATIONS_DRAW_H__
#define __NOTIFICATIONS_DRAW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "application.h"

//传递通知信息的队列
extern osMessageQueueId_t Notification_QueueHandle;
    
//弹窗显示时间（单位：毫秒）
extern int notification_showtime;
    
//向弹窗队列发送一条消息
void Notification_Send(const char *msg);
    
//处理弹窗相关操作
void Notification_Process(void);
    
#ifdef __cplusplus
}
#endif

#endif
 