/*菜单驱动任务函数
 *负责U8G2与UI的初始化与运行
 *
 *
 */
#include "application.h"

ui_t ui;    

void Menu_Task(void *argument)
{
    // 初始化U8g2库，为OLED显示做准备
    u8g2_InitDisplay(&u8g2);
    Disp_SetPowerSave(0);
    Disp_ClearBuffer();
    Disp_SetContrast2(Contrast);
    
    MiaoUi_Setup(&ui);
    
    for(;;)
    {   
        ui_loop(&ui);//开屏幕的情况下才运行菜单函数
        osDelay(pdMS_TO_TICKS(2));//硬件SPI 10MHz DMA情况下延时10ms，硬件I2C 800K下延时2ms
    }
}