/*包含所有的与UI界面有关的函数实现
 *也提供后台运行的监控函数，提供手势
 */
#include "application.h"
#include "math.h"

/**************************运行在菜单任务中的函数******************************/
//Tool菜单项中Gradienter（水平仪）功能的实现函数
void Gradienter(ui_t *ui){
    char buf[16];                                                               //显示用缓冲区数组
    uint8_t color;                                                              //设置颜色
    
    while(1){
        //两种颜色下的清屏
        Disp_SetDrawColor(&ui->bgColor);
        Disp_DrawBox(0, 0, UI_HOR_RES, UI_VER_RES);
        color = ui->bgColor^ 0x01;
        Disp_SetDrawColor(&color);
        
        Disp_SetFont(font_menu_main_h12w6);
        
        sprintf(buf, "X:%.3f m/s^2", SC7A20_readaccel_x()- 0.9f);
        Disp_DrawStr(10, 18, buf);
        sprintf(buf, "Y:%.3f m/s^2", SC7A20_readaccel_y()+ 0.8f);
        Disp_DrawStr(10, 30, buf);
        sprintf(buf, "Z:%.3f m/s^2", SC7A20_readaccel_z()- 0.9f);
        Disp_DrawStr(10, 42, buf);
        sprintf(buf, "Tol:%.3f m/s^2", sqrtf((SC7A20_readaccel_z()- 0.9f)*(SC7A20_readaccel_z()- 0.9f)+(SC7A20_readaccel_y()+ 0.8f)*(SC7A20_readaccel_y()+ 0.8f)+(SC7A20_readaccel_x()- 0.9f)*(SC7A20_readaccel_x()- 0.9f)));
        Disp_DrawStr(10, 54, buf);
        
        switch(indevScan()){
            case UI_ACTION_BACK://退出收音机操作界面（收音机仍在后台运行）
                ui->action = UI_ACTION_ENTER;
                return;
            case UI_ACTION_ENTER://进入设置页面
                break;
            case UI_ACTION_NONE:
                break;
            default:
                break;
        }
        
        Disp_SendBuffer();
        osDelay(pdMS_TO_TICKS(10));
    }
}

/***********************以下函数须在ui_conf.c中调用****************************/
//与MiaoUI的接口函数，用于注册水平仪的设置项
//创建加速度计模块所需的参数
void Create_Gradienter_Parameters (ui_t *ui){
    
}
//将加速度计模块的对象添加到菜单中
void Add_Gradienter_Items(ui_page_t *ParentPage){
    
}

/**************************运行在后台任务中的函数******************************/
//根据读到的加速度计数据，发出手势数据，更新屏幕旋转方向
void Gesture_Process(void){
    
}
