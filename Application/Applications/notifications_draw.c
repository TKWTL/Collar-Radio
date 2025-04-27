#include "application.h"

//弹窗显示时间（单位：毫秒）
int notification_showtime = 1500;

//定义弹窗提示文本的字符串数组
const char* notification_text[] = {
    "Battery Low!!",
    "Charge Started.",
    "Battery Fulled.",
    "EarPlug Inserted.",
    "EarPlug Removed."
};

//向弹窗队列发送一条消息
void Notification_Send(uint8_t index){
    osMessageQueuePut(Notification_QueueHandle, &index, NULL, 0);
}

/*
 *弹窗功能实现任务函数
 *循环检查队列，无内容则阻塞线程
 *有内容时，阻塞Menu_Task，内容的值作为索引值查询notifications_draw.h中定义的字符串数组，并通过动画效果显示出来
 *在用户按键或者到达了系统设置的展示时间后退出
 */
void Notification_Process(void){
    osStatus_t status;
    uint8_t notification_index;
    uint8_t bgC = ui.bgColor;
    int notification_timer;
    
    status = osMessageQueueGet(Notification_QueueHandle, &notification_index, NULL, 0);
    if(status == osOK){
        Disp_SendBuffer();
        vTaskSuspend(MenuHandle);
        //显示
        PopUp_Show(2, 10, 122, 42);
        Disp_SetFont(font_menu_main_h12w6);
        Disp_DrawStr(5, 34, notification_text[notification_index]);
        Disp_SendBuffer();
        
        //延时展示，有按键时退出
        for(notification_timer = notification_showtime; notification_timer > 0; notification_timer -= 100){
            if(indevScan() != UI_ACTION_NONE) break;
            osDelay(pdMS_TO_TICKS(100));
        }
        
        vTaskResume(MenuHandle);
    }
}
    