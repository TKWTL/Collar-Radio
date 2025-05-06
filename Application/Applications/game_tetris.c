#include "application.h"
#include "stdlib.h"

void Game_Tetris(ui_t *ui){
    //切换屏幕显示方向
    Disp_SetFont(font_menu_main_h12w6);//设置字体
    srand(sTime.Seconds + sTime.Date + sTime.Month + sTime.Year);//初始化种子
    //init_game();//初始化地图
    
    while(1){
        // 处理游戏输入
        switch (indevScan()) {
            case UI_ACTION_PLUS:
                break;
            case UI_ACTION_MINUS:
                break;
            case UI_ACTION_UP:
                ui->action = UI_ACTION_ENTER;
                return;
            case UI_ACTION_DOWN:
                break;
            case UI_ACTION_BACK:
                break;
            case UI_ACTION_ENTER:
                break;
            case UI_ACTION_NONE:
                break;
        }
        Disp_SendBuffer();
        osDelay(pdMS_TO_TICKS(10));
    }
}