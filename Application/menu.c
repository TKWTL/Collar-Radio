/*
 *
 *
 *
 */
#include "application.h"

extern u8g2_t u8g2;

ui_t ui;    

void Menu_Task(void *argument)
{
    // 初始化U8g2库，为OLED显示做准备
    u8g2_InitDisplay(&u8g2);
    u8g2_SetPowerSave(&u8g2, 0);
    u8g2_ClearBuffer(&u8g2);
    u8g2_SetContrast(&u8g2, 15);
    
    MiaoUi_Setup(&ui);
    
    for(;;)
    {   
        ui_loop(&ui);
        osDelay(pdMS_TO_TICKS(10));
    }
}