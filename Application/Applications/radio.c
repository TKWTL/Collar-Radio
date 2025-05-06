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
uint16_t station_freq[MAX_STATION_NUM];//静态存储电台频率的数组，低频率的索引值较小
uint8_t station_num = 0;//数组中有效电台的数目
uint8_t global_seeking = 0;//全局搜索标志位

/*查询数组中是否存在该电台频率
 *存在返回1，否则返回0
 */
uint8_t Station_IsExist(uint16_t frequency){
    uint8_t i;
    for (i = 0; i < station_num; i++) {
        if (station_freq[i] == frequency) return 1;
        if (station_freq[i] > frequency) return 0; //利用升序提前退出
    }
    return 0;
}

/*向数组中插入一个电台频率
 *成功插入返回1，否则返回0
 *超过最大电台数量或已有此频率时插入失败
 */
uint8_t Station_Insert(uint16_t frequency){
    uint8_t i, j;
    if(station_num >= MAX_STATION_NUM) return 0; //检查是否超过最大电台数量
    for(i = 0;i < station_num; i++){
        if(station_freq[i] == frequency) return 0;//已有该频率，无法插入
        else if(station_freq[i] > frequency){
            //在i位置插入，需移位
            for(j = station_num; j > i; j--){
                station_freq[j] = station_freq[j-1];
            }
            station_freq[i] = frequency;
            station_num++;
            return 1; //插入成功
        }
    }
    //插入到末尾
    station_freq[station_num] = frequency;
    station_num++;
    return 1; //插入成功
}

/*向数组中移除一个电台频率
 *成功移除返回1，否则返回0
 *超过电台数量为0或没有此频率时移除失败
 */
uint8_t Station_Remove(uint16_t frequency){
    uint8_t i, j;
    for(i = 0; i < station_num; i++){
        if(station_freq[i] == frequency){
            //找到频率，删除
            for(j = i; j < station_num - 1; j++){
                station_freq[j] = station_freq[j + 1];
            }
            station_num--;
            return 1; //删除成功了
        }
        else if(station_freq[i] > frequency) return 0;//由于数组已排序，frequency不在数组中
    }
    return 0;//未找到
}

/*启动一次全局搜索
 */
void InitGlobalSeek(void){
    global_seeking = 1;
    station_num = 0;
    RDA5807_SetFreq(87* 40);
    RDA5807_SeekDirection(SEEK_UP);
    RDA5807_StartSeek();
}

//用于管理RDA5807芯片开关状态的接口变量
RadioPowerState_t RadioPowerMode = RADIO_AUTO;

// 定义菜单项数量
#define NUM_MENU_ITEMS 5
// 固定标签数组
const char* fixed_labels[NUM_MENU_ITEMS] = {
    "Return",          // i=0
    "Power Mode:",     // i=1
    "Radio Mode:",     // i=2
    NULL,               // i=3，没有固定标签
    "Exit Radio Page"
};
#define DYNAMIC_OPTIONS_X   74

//收音机运行的界面函数，处理显示与按键
void Radio_Run(ui_t *ui){
    char buf[16];                                                               //显示用缓冲区数组
    uint8_t color;                                                              //设置颜色
    uint8_t i, y;
    
    static uint16_t frequency = 87* 40;                                         //频率，单位25KHz
    static uint8_t freq_align;                                                  //屏幕居中对齐用变量
    static uint16_t volume = 0;                                                 //音量
    static uint8_t isSeeking = 0;                                               //是否在搜台/调谐中
    uint8_t batterylvl;                                                         //电池实际显示格数，用于实现充电动效
    int16_t disp_value1 = 12;                                                   //动效变量
    static RadioMode_t RadioMode = MODE_STA;                                    //收音机的模式，默认预设电台
    uint8_t setting_mode = 0;                                                   //设置位切换标志
    uint8_t menu_index = 0;                                                     //设置项位置
    
    
    while(1){
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
                if(RadioMode == MODE_FREQ) Disp_DrawStr(12,37, "Tuning....");
                else Disp_DrawStr(12,37, "Seeking...");
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
            //显示频率条
            Disp_DrawPixel((frequency- 3440)* 5/ 36, 43);//以点的形式显示当前频率
            Disp_DrawLine(0, 45, UI_HOR_RES, 45);
            for(batterylvl = 0; batterylvl < station_num; batterylvl++){
                Disp_DrawPixel((station_freq[batterylvl]- 3440)* 5/ 36, 47);//以点的形式显示已存台
                if(station_freq[batterylvl] == frequency)
                    Disp_DrawPixel((frequency- 3440)* 5/ 36, 46);//当前频率等于一个预设台时额外标识出来
                    Disp_DrawPixel((frequency- 3440)* 5/ 36, 48);
            }
            
            disp_value1 = (int16_t)UI_Animation(0, (float)disp_value1, &ui->animation.cursor_ani);
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
                case UI_ACTION_BACK://退出收音机操作界面（收音机仍在后台运行）
                    ui->action = UI_ACTION_ENTER;
                    return;
                case UI_ACTION_UP://减小频率
                    switch(RadioMode){
                        case MODE_SEARCH:
                            if(isSeeking == 0){
                                RDA5807_SeekDirection(SEEK_DOWN);
                                RDA5807_StartSeek();
                            }
                            else RDA5807_InterruptSeek();
                            break;
                        case MODE_STA:
                            if(station_num == 0) InitGlobalSeek();//没有预存台时立刻开始一次全局搜索
                            else if(global_seeking == 1) global_seeking = 0;//全局搜索中则暂停搜台
                            else{
                                batterylvl = 0;  
                                while(batterylvl < station_num && station_freq[batterylvl] < frequency) batterylvl++;  
                                frequency = (batterylvl == 0) ? station_freq[station_num- 1] : station_freq[batterylvl- 1];//若无更小频率，循环到最后一个电台
                                RDA5807_SetFreq(frequency);
                            }
                            break;
                        case MODE_FREQ:
                            if(frequency <= 87* 40) frequency = 108* 40;
                            else frequency -= 4;
                            RDA5807_SetFreq(frequency);
                            break;
                        default:
                            break;
                    }
                    break;
                case UI_ACTION_DOWN://增大频率
                    switch(RadioMode){
                        case MODE_SEARCH:
                            if(isSeeking == 0){
                                RDA5807_SeekDirection(SEEK_UP);
                                RDA5807_StartSeek();
                            }
                            else RDA5807_InterruptSeek();
                            break;
                        case MODE_STA:
                            if(station_num == 0) InitGlobalSeek();//没有预存台时立刻开始一次全局搜索
                            else if(global_seeking == 1) global_seeking = 0;//全局搜索中则暂停搜台
                            else{
                                for(batterylvl = 0; batterylvl < station_num; batterylvl++) 
                                    if(station_freq[batterylvl] > frequency) 
                                        break;
                                frequency = station_freq[(batterylvl < station_num) ? batterylvl : 0];// 若无更大频率，循环到第一个电台
                                RDA5807_SetFreq(frequency);
                            }
                            break;
                        case MODE_FREQ:
                            if(frequency >= 108* 40) frequency = 87* 40;
                            else frequency += 4;
                            RDA5807_SetFreq(frequency);
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
                case UI_ACTION_ENTER://进入设置页面
                    setting_mode = 1;
                    global_seeking = 0;
                    break;
                case UI_ACTION_NONE:
                    break;
            }
            if(global_seeking){//触发了全局扫描，该操作无法在设置页面进行
                if(isSeeking == 0){
                    Station_Insert(frequency);
                    RDA5807_StartSeek();
                }
                if(station_num >= MAX_STATION_NUM || frequency >= 108* 40){//达到最大电台数量或到达频率最大值
                    global_seeking = 0;
                    RDA5807_InterruptSeek();
                }
            }
        }
        else{//设置页面
            // 绘制标题和水平线
            Disp_SetFont(font_menu_main_h12w6);
            Disp_DrawLine(0, 6, UI_HOR_RES, 6);
            Disp_DrawStr(19, 10, "-Radio Setting:-");

            // 绘制菜单项
            for (i = 0; i < NUM_MENU_ITEMS; i++) {
                y = 20 + i * 10; // 计算 Y 坐标
                // 绘制固定标签（如果存在）
                if (i != 3) Disp_DrawStr(2, y, fixed_labels[i]);
                // 绘制动态文本或值
                if (i == 1) {
                    // Power Mode 的值
                    switch (RadioPowerMode) {
                        case RADIO_ON:  Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "ON");   break;
                        case RADIO_AUTO:Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "AUTO"); break;
                        case RADIO_OFF: Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "OFF");  break;
                    }
                } else if (i == 2) {
                    // Radio Mode 的值
                    if (RadioMode == MODE_SEARCH)       Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "Search");
                    else if (RadioMode == MODE_FREQ)    Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "Frequency");
                    else                                Disp_DrawStr(DYNAMIC_OPTIONS_X, y, "Preset");
                } else if (i == 3) {
                    // 动态文本（根据 RadioMode 和其他条件）
                    if (RadioMode == MODE_STA) Disp_DrawStr(1, y, "-All Clear and Seek-");
                    else {
                        if (Station_IsExist(frequency)) Disp_DrawStr(1, y, "-Remove from List-");
                        else Disp_DrawStr(1, y, "-Add to station List-");
                    }
                }
                if(i == menu_index){
                    if(i != 3) ui->cursor.nowWide = (int)UI_Animation(strlen(fixed_labels[i])* 6 + 2, (float)ui->cursor.nowWide, &ui->animation.optionbarPos_ani);
                    else{
                        if(RadioMode == MODE_STA)
                            ui->cursor.nowWide = (int)UI_Animation(strlen("-All Clear and Seek-")* 6 + 2, (float)ui->cursor.nowWide, &ui->animation.optionbarPos_ani);
                        else{
                            if(Station_IsExist(frequency))
                                ui->cursor.nowWide = (int)UI_Animation(strlen("-Remove from List-")* 6 + 2, (float)ui->cursor.nowWide, &ui->animation.optionbarPos_ani);
                            else
                                ui->cursor.nowWide = (int)UI_Animation(strlen("-Add to station List-")* 6 + 2, (float)ui->cursor.nowWide, &ui->animation.optionbarPos_ani);
                        }
                    }
                }
            }

            // 绘制光标
            disp_value1 = (int16_t)UI_Animation(10+ menu_index* 10, (float)disp_value1, &ui->animation.cursor_ani);
            color = 0x02;
            Disp_SetDrawColor(&color);
            Disp_DrawRBox(0, disp_value1 + 1, ui->cursor.nowWide, 10, 0);
            color = ui->bgColor ^ 0x01;
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
                    if(menu_index == 0) menu_index = NUM_MENU_ITEMS- 1;
                    else menu_index--;
                    break;
                case UI_ACTION_MINUS:
                    if(menu_index == NUM_MENU_ITEMS- 1) menu_index = 0;
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
                        case 3:
                            if(RadioMode == MODE_STA){//立刻启动扫描并退回主界面
                                InitGlobalSeek();
                                setting_mode = 0;
                                disp_value1 = 12;
                            }
                            else{
                                if(Station_IsExist(frequency) == 0) Station_Insert(frequency);
                                else Station_Remove(frequency);
                            }
                            break;
                        case 4://退出收音机操作界面（收音机仍在后台运行）
                            ui->action = UI_ACTION_ENTER;
                            return;
                    }
                    break;
                case UI_ACTION_NONE:
                    break;
            }
        }

        Disp_SendBuffer();
        osDelay(pdMS_TO_TICKS(10));
    }//while(1) end
}

