/*收音机页面函数实现
 *
 */
#include "application.h"

const uint8_t signal[] = {//信号图片，XBM格式
0x7f,0x49,0x2a,0x1c,0x08,0x08,0x08,0x08,0x08
};
#define BATTERY_X   111
#define BATTERY_Y   2
const uint8_t battery[] = {//电池图片，XBM格式
0xfc,0xff,0x04,0x80,0x07,0x80,0x05,0x80,0x05,0x80,0x07,0x80,0x04,0x80,0xfc,0xff
};
#define SPEAKER_X   69
#define SPEAKER_Y   1
const uint8_t speaker[] = {//音量图片，XBM格式
0x30,0x28,0x27,0x25,0x25,0x25,0x27,0x28,0x30
};

#define MAX_STATION_NUM 50  //最大存台数量
uint16_t station_freq[MAX_STATION_NUM];//静态存储电台频率的数组
uint8_t station_num = 0;//数组中有效电台的数目

RadioPowerState_t RadioPowerMode = RADIO_ON;

void Radio_Run(ui_t *ui){
    static uint16_t frequency = 86* 40;//频率，单位25KHz
    static uint8_t seek_dir = SEEK_DOWN;//默认向下搜台
    static uint8_t freq_align;//屏幕居中对齐用变量
    static uint16_t volume = 0;//音量
    static uint8_t isSeeking = 0;
    char buf[16];//显示用缓冲区数组
    uint8_t color;//设置颜色
    uint8_t batterylvl;//电池实际显示格数，用于实现充电动效
    
    int16_t disp_value1 = 12;//动效变量
    static RadioMode_t RadioMode = MODE_SEARCH;//收音机模式
    uint8_t setting_mode = 0;//设置位切换标志
    uint8_t menu_index = 0;//设置项位置
    
    
    while(1){
        /***************************屏幕显示区*********************************/
        //两种颜色下的清屏
        Disp_SetDrawColor(&ui->bgColor);
        Disp_DrawBox(0, 0, UI_HOR_RES, UI_VER_RES);
        color = ui->bgColor^ 0x01;
        Disp_SetDrawColor(&color);
        
        if(setting_mode == 0){
            //读取信息
            if(RDA5807_CheckSTC()) isSeeking = 0;
            else isSeeking = 1;
            frequency = RDA5807_RegToFreq(RDA5807_GetFreqReg());
            //频率信息显示
            Disp_SetFont(font_h24w12);
            if(isSeeking){
                Disp_DrawStr(12,37, "Seeking...");
                Disp_SetFont(font_menu_main_h12w6);
            }
            else{
                freq_align = 5;
                if(frequency>= 4000) freq_align++;//大于100Mhz
                if(frequency% 2 == 1){//多出0.025的一位数
                    freq_align++;
                    sprintf(buf, "%3.3f", frequency* 0.025f);
                }
                else sprintf(buf, "%3.2f", frequency* 0.025f);
                Disp_DrawStr(61- freq_align* 6, 38, buf);//Y轴需要减6格
                Disp_SetFont(font_menu_main_h12w6);
                Disp_DrawStr(63+ freq_align* 6, 38, "MHz");
            }
            Disp_DrawPixel((frequency- 3440)* 5/ 36, 43);
            Disp_DrawLine(0, 45, UI_HOR_RES, 45);
            
            disp_value1 = (int16_t)UI_Animation(0, (float)disp_value1, &ui->animation.scrollbar_ani);
            //信号信息显示
            Disp_DrawXBMP(1, 1- disp_value1, 7, 9, signal);
            sprintf(buf, ":%d", RDA5807_GetSigLvl());
            Disp_DrawStr(8, 10- disp_value1, buf);
            
            //音量信息显示
            Disp_DrawXBMP(SPEAKER_X, SPEAKER_Y- disp_value1, 6, 9, speaker);
            sprintf(buf, ":%d%%", (volume+ 1)* 100/ 16);
            Disp_DrawStr(SPEAKER_X+ 6, SPEAKER_Y+ 9- disp_value1, buf);
            
            if(LL_GPIO_IsInputPinSet(INDET_GPIO_Port, INDET_Pin)) Disp_DrawStr(48, 10- disp_value1, "OFF");
            else Disp_DrawStr(48, 10- disp_value1, "ON");
            
            //电量显示效果：
            //超低电压下电池框闪烁
            if(Vbattery > 3.2 || (HAL_GetTick()/ 800)% 2 == 0 || eChargeState == CHARGING) 
                Disp_DrawXBMP(BATTERY_X, BATTERY_Y- disp_value1, 16, 8, battery);
            //充电模式下显示动画，从当前电量格逐渐递增到满格，周而复始
            batterylvl = Battery_level;
            if(eChargeState == CHARGING){
                if(Battery_level > 0) batterylvl--;
                batterylvl += (HAL_GetTick()/ 800)% (5- batterylvl);
            }
            //放电模式电压由高到低，电池格数递减
            Disp_DrawBox(BATTERY_X+ 15- 3* batterylvl, BATTERY_Y+ 1- disp_value1, batterylvl* 3, 6);
            
            //显示时间
            sprintf(buf, "%02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);
            Disp_DrawStr(1, 63+ disp_value1, buf);
            sprintf(buf, "20%02d/%02d/%02d", sTime.Year, sTime.Month, sTime.Date);
            Disp_DrawStr(68, 63+ disp_value1, buf);
        
            
            switch(indevScan()){
                case UI_ACTION_BACK:
                    ui->action = UI_ACTION_ENTER;
                    return;
                case UI_ACTION_UP:
                    switch(RadioMode){
                        case MODE_SEARCH:
                            if(seek_dir != SEEK_DOWN){
                                seek_dir = SEEK_DOWN;
                                RDA5807_SeekDirection(seek_dir);
                            }
                            RDA5807_StartSeek();
                            break;
                        case MODE_STA:
                            break;
                        case MODE_FREQ:
                            break;
                        default:
                            break;
                    }
                    break;
                case UI_ACTION_DOWN:
                    switch(RadioMode){
                        case MODE_SEARCH:
                            if(seek_dir != SEEK_UP){
                                seek_dir = SEEK_UP;
                                RDA5807_SeekDirection(seek_dir);
                            }
                            RDA5807_StartSeek();
                            break;
                        case MODE_STA:
                            break;
                        case MODE_FREQ:
                            break;
                        default:
                            break;
                    }
                    break;
                case UI_ACTION_PLUS:
                    volume = RDA5807_IncVolume();
                    break;
                case UI_ACTION_MINUS:
                    volume = RDA5807_DecVolume();
                    break;
                case UI_ACTION_ENTER:
                    setting_mode = 1;
                    break;
                case UI_ACTION_NONE:
                    break;
            }
        }
        else{
            Disp_SetFont(font_menu_main_h12w6);
            Disp_DrawLine(0, 6, UI_HOR_RES, 6);
            Disp_DrawStr(28, 10, "Radio Setting");
            Disp_DrawStr(1, 23, "*Return");
            Disp_DrawStr(1, 36, "*Power Mode:");
            switch(RadioPowerMode){
                case RADIO_ON:
                    Disp_DrawStr(73, 36, "ON");
                    break;
                case RADIO_AUTO:
                    Disp_DrawStr(73, 36, "AUTO");
                    break;
                case RADIO_OFF:
                    Disp_DrawStr(73, 36, "OFF");
                    break;
            }
            Disp_DrawStr(1, 49, "*Radio Mode:");
            switch(RadioMode){
                case MODE_SEARCH:
                    Disp_DrawStr(73, 49, "Search");
                    Disp_DrawStr(1, 62, "*Add to station List");
                    break;
                case MODE_FREQ:
                    Disp_DrawStr(73, 49, "Frequency");
                    Disp_DrawStr(1, 62, "*Add to station List");
                    break;
                case MODE_STA:
                    Disp_DrawStr(73, 49, "Preset");
                    Disp_DrawStr(1, 62, "*Clear and Seek");
                    break;
            }
            disp_value1 = (int16_t)UI_Animation((menu_index+ 1)* 13, (float)disp_value1, &ui->animation.scrollbar_ani);
            color = 0x02;
            Disp_SetDrawColor(&color);
            Disp_DrawRBox(0, disp_value1, UI_HOR_RES, 12, 0);
            color = ui->bgColor^ 0x01;
            Disp_SetDrawColor(&color);
            
            switch(indevScan()){
                case UI_ACTION_BACK:
                    setting_mode = 0;
                    disp_value1 = 12;
                    break;
                case UI_ACTION_UP:
                    break;
                case UI_ACTION_DOWN:
                    break;
                case UI_ACTION_PLUS:
                    if(menu_index == 0) menu_index = 3;
                    else menu_index--;
                    break;
                case UI_ACTION_MINUS:
                    if(menu_index == 3) menu_index = 0;
                    else menu_index++;
                    break;
                case UI_ACTION_ENTER:
                    switch(menu_index){
                        case 0:
                            setting_mode = 0;
                            disp_value1 = 12;
                            break;
                        case 1:
                            if(RadioPowerMode == RADIO_OFF) RadioPowerMode = RADIO_ON;
                            else RadioPowerMode++;
                            break;
                        case 2:
                            if(RadioMode == MODE_FREQ) RadioMode = MODE_SEARCH;
                            else RadioMode++;
                            break;
                    }
                    break;
                case UI_ACTION_NONE:
                    break;
            }
        }

        Disp_SendBuffer();
        osDelay(pdMS_TO_TICKS(10));
    }
}

