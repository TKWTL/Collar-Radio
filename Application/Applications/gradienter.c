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
    
    Disp_SetFont(font_menu_main_h12w6);
    while(1){
        //两种颜色下的清屏
        Disp_SetDrawColor(&ui->bgColor);
        Disp_DrawBox(0, 0, UI_HOR_RES, UI_VER_RES);
        color = ui->bgColor^ 0x01;
        Disp_SetDrawColor(&color);
        
        
        Disp_DrawCircle(31, UI_VER_RES/ 2, 30, U8G2_DRAW_ALL);
        Disp_DrawCircle(31, UI_VER_RES/ 2, 31, U8G2_DRAW_ALL);
        Disp_DrawCircle(31+ 12* atanf(SC7A20_readaccel_x()), UI_VER_RES/ 2- 12* atanf(SC7A20_readaccel_y()), 7, U8G2_DRAW_ALL);
        
        Disp_DrawStr(67, 36, "Altitude:");
        sprintf(buf, "%.3f m", BMP180_Altitude);
        Disp_DrawStr(73, 48, buf);
        
        switch(indevScan()){
            case UI_ACTION_BACK://退出界面
                ui->action = UI_ACTION_ENTER;
                return;
            case UI_ACTION_ENTER://进入设置页面
                break;
            default:
                break;
        }
        
        Disp_SendBuffer();
        osDelay(pdMS_TO_TICKS(10));
    }
}

const uint8_t ARROW0[] = {
    0x00,0x02,0x00,0x00,0x00,0x03,0x00,0x00,0x80,0x03,0x00,0x00,0xc0,0x02,0x00,0x00,
    0x60,0xfe,0xff,0x01,0x30,0xfe,0xff,0x00,0x18,0x00,0x60,0x00,0x0c,0x00,0x30,0x00,
    0x06,0x00,0x18,0x00,0x03,0x00,0x0c,0x00,0x06,0x00,0x18,0x00,0x0c,0x00,0x30,0x00,
    0x18,0x00,0x60,0x00,0x30,0xfe,0xff,0x00,0x60,0xfe,0xff,0x01,0xc0,0x02,0x00,0x00,
    0x80,0x03,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00
};
const uint8_t ARROW1[] = {
    0x00,0x30,0x18,0x00,0x00,0x00,0x70,0x1c,0x00,0x00,0x00,0xf0,0x1e,0x00,0x00,0x00,
    0xb0,0x1b,0x00,0x00,0x00,0x30,0x19,0x00,0x00,0x00,0x30,0x18,0x00,0x00,0x00,0x30,
    0x18,0x00,0x00,0x00,0x30,0x18,0x00,0x00,0xff,0x3f,0xf8,0xff,0x01,0xff,0x1f,0xf0,
    0xff,0x01,0x03,0x00,0x00,0x80,0x01,0x0e,0x00,0x00,0xe0,0x00,0x3c,0x00,0x00,0x78,
    0x00,0xf0,0x00,0x00,0x1e,0x00,0xc0,0x03,0x80,0x07,0x00,0x00,0x0f,0xe0,0x01,0x00,
    0x00,0x3c,0x78,0x00,0x00,0x00,0xf0,0x1e,0x00,0x00,0x00,0xc0,0x07,0x00,0x00,0x00,
    0x00,0x01,0x00,0x00
};

//Tool菜单项中加速度计校准功能的实现函数
//基于六面体静态校准法
void Accel_Calibration(ui_t *ui){
    char buf[16];                                                               //显示用缓冲区数组
    uint8_t color;                                                              //设置颜色
    uint8_t state = 0;
    float X0, X1;
    
    Disp_SetFont(font_menu_main_h12w6);
    while(1){
        //两种颜色下的清屏
        Disp_SetDrawColor(&ui->bgColor);
        Disp_DrawBox(0, 0, UI_HOR_RES, UI_VER_RES);
        color = ui->bgColor^ 0x01;
        Disp_SetDrawColor(&color);
        
        switch(state){
            case 0:
                Disp_ResumeRotation(ui);
                Disp_DrawStr(16, 10, "Calibration Done.");
                sprintf(buf, "X:%.3f m/s^2", SC7A20_readaccel_x());
                Disp_DrawStr(22, 21, buf);
                sprintf(buf, "Y:%.3f m/s^2", SC7A20_readaccel_y());
                Disp_DrawStr(22, 31, buf);
                sprintf(buf, "Z:%.3f m/s^2", SC7A20_readaccel_z());
                Disp_DrawStr(22, 41, buf);
                sprintf(buf, "Tol:%.3f m/s^2", sqrtf((SC7A20_readaccel_z())*(SC7A20_readaccel_z())+(SC7A20_readaccel_y())*(SC7A20_readaccel_y())+(SC7A20_readaccel_x())*(SC7A20_readaccel_x())));
                Disp_DrawStr(10, 51, buf);
                Disp_DrawStr(4, 62, "Any Key to Calibrate");
            break;
            case 1:
                Disp_SetRotation(0);
                Disp_DrawStr(4, 11, "Calibrating. Phase:1");
                Disp_DrawStr(44, 38, "GROUND:");
                Disp_DrawXBMP(3, 24, 25, 19, ARROW0);
                sprintf(buf, "X:%.3f m/s^2", SC7A20_readaccel_x());
                Disp_DrawStr(22, 62, buf);
            break;
            case 2:
                Disp_SetRotation(1);
                Disp_DrawStr(4, 11, "Calibrating. Phase:2");
                Disp_DrawStr(44, 38, "GROUND:");
                Disp_DrawXBMP(3, 24, 25, 19, ARROW0);
                sprintf(buf, "X:%.3f m/s^2", SC7A20_readaccel_x());
                Disp_DrawStr(22, 62, buf);
            break;
            case 3:
                Disp_SetRotation(0);
                Disp_DrawStr(4, 11, "Calibrating. Phase:3");
                sprintf(buf, "Y:%.3f m/s^2", SC7A20_readaccel_y());
                Disp_DrawStr(22, 25, buf);
                Disp_DrawStr(44, 38, "GROUND:");
                Disp_DrawXBMP(46, 42, 33, 20, ARROW1);
            break;
            case 4:
                Disp_SetRotation(1);
                Disp_DrawStr(4, 11, "Calibrating. Phase:4");
                sprintf(buf, "Y:%.3f m/s^2", SC7A20_readaccel_y());
                Disp_DrawStr(22, 25, buf);
                Disp_DrawStr(44, 38, "GROUND:");
                Disp_DrawXBMP(46, 42, 33, 20, ARROW1);
            break;
            case 5:
                Disp_SetRotation(0);
                Disp_DrawStr(4, 11, "Calibrating. Phase:5");
                Disp_DrawStr(26, 38, "->FACE UP<-");
                sprintf(buf, "Z:%.3f m/s^2", SC7A20_readaccel_z());
                Disp_DrawStr(22, 62, buf);
            break;
            case 6:
                Disp_SetRotation(0);
                Disp_DrawStr(4, 11, "Calibrating. Phase:6");
                Disp_DrawStr(20, 38, "->FACE DOWN<-");
                sprintf(buf, "Z:%.3f m/s^2", SC7A20_readaccel_z());
                Disp_DrawStr(22, 62, buf);
            break;
            default:
            break;
        }
        
        switch(indevScan()){
            case UI_ACTION_BACK://退出界面
                Disp_ResumeRotation(ui);
                ui->action = UI_ACTION_ENTER;
                return;
            case UI_ACTION_UP:
            case UI_ACTION_DOWN:
            case UI_ACTION_PLUS:
            case UI_ACTION_MINUS:
            case UI_ACTION_ENTER://进入设置页面
                switch(state){
                    case 0:
                        state++;
                    break;
                    case 1:
                        X0 = SC7A20_readaccel_x();
                        state++;
                    break;
                    case 2:
                        X1 = SC7A20_readaccel_x();
                        SC7A20_Compensate_X(X0, X1);
                        state++;
                    break;
                    case 3:
                        X0 = SC7A20_readaccel_y();
                        state++;
                    break;
                    case 4:
                        X1 = SC7A20_readaccel_y();
                        SC7A20_Compensate_Y(X0, X1);
                        state++;
                    break;
                    case 5:
                        X0 = SC7A20_readaccel_z();
                        state++;
                    break;
                    case 6:
                        X1 = SC7A20_readaccel_z();
                        SC7A20_Compensate_Z(X0, X1);
                        state = 0;
                    break;
                    default:
                    break;
                }
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
