#include "application.h"


void Button_Handle(void *argument)
{
    uint8_t i;
    uint16_t btn;
    Key_Init();
    for(;;)
    {
        Key_DebounceService_10ms();
        Key_Scand();
        if(Unstable_of_Wake == 0){//判断是否刚刚唤醒，是则不响应按键，避免误操作
            for(i = 0; i < KeyIndex_Max; i++){
                if(KEY_GetDASClick(i)){
                    btn = (i << 8)| 0x01;
                    osMessageQueuePut(Button_QueueHandle, &btn, 0, osWaitForever);
                    UserOperationDetected();
                }
            }
        }
        osDelay(pdMS_TO_TICKS(10));
    }
}
