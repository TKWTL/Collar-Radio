/*
 * This file is part of the MiaoUI Library.
 *
 * Copyright (c) 2025, JFeng-Z, <2834294740@qq.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Created on: 2025-02-08
 */
#include "ui_conf.h"
#include "ui.h"
#include "dispDriver.h"
#include "image.h"//使用的图片资源

//需要调用的项目函数
#include "application.h"

/*Page*/
ui_page_t Menu_Page;
ui_page_t Setting_Page, Display_Page, Time_Page, SystemSetting_Page;
ui_page_t Stat_Page;
ui_page_t Game_Page;
ui_page_t Tool_Page;

/*item */
ui_item_t HomeHead_Item;

ui_item_t Setting_Item, SettingHead_Item;
ui_item_t Display_Item, DisplayHead_Item, Contrast_Item, MenuColor_Item, SleepTime_Item, AutoSleep_Item;
ui_item_t Time_Item, TimeHead_Item, Year_Item, Month_Item, Days_Item, Hour_Item, Minute_Item, Second_Item;
ui_item_t SystemSetting_Item, SystemHead_Item, NotificationTime_Item, WakeInactiveTime_Item, Reset_Item;

ui_item_t Statistic_Item, StatHead_Item;
ui_item_t VBAT_Item, ICharge_Item, Tj_Item, Tair_Item, Pair_Item;


ui_item_t Game_Item, GameHead_Item;
ui_item_t Tetris_Item;
ui_item_t Snake_Item;
ui_item_t Dino_Item;

ui_item_t Tool_Item, ToolHead_Item;
ui_item_t Gradienter_Item, I2C_Scanner_Item, Burn_in_Test_Item;

ui_item_t Github_Item;

/**
 * 在此建立所需显示或更改的数据
 * 无参数
 * 无返回值
 */
void Create_Parameter(ui_t *ui)
{    
    //设置区    
    static ui_data_t Contrast_data;
    Contrast_data.name = "Contrast";
    Contrast_data.ptr = &Contrast;
    Contrast_data.function = Disp_SetContrast;
    Contrast_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Contrast_data.dataType = UI_DATA_INT;
    Contrast_data.actionType = UI_DATA_ACTION_RW;
    Contrast_data.max = 63;
    Contrast_data.min = 1;
    Contrast_data.step = 1;
    static ui_element_t Contrast_element;
    Contrast_element.data = &Contrast_data;
    Create_element(&Contrast_Item, &Contrast_element);
    
    static ui_data_t MenuColor_data;
    MenuColor_data.name = "MenuColor";
    MenuColor_data.ptr = &ui->bgColor;
    MenuColor_data.dataType = UI_DATA_SWITCH;
    MenuColor_data.actionType = UI_DATA_ACTION_RW;
    static ui_element_t MenuColor_element;
    MenuColor_element.data = &MenuColor_data;
    Create_element(&MenuColor_Item, &MenuColor_element);
    
    static ui_data_t SleepTime_data;
    SleepTime_data.name = "Auto Sleep Time";
    SleepTime_data.ptr = &Sleep_time;
    SleepTime_data.dataType = UI_DATA_INT;
    SleepTime_data.actionType = UI_DATA_ACTION_RW;
    SleepTime_data.max = 600;
    SleepTime_data.min = 10;
    SleepTime_data.step = 5;
    static ui_element_t SleepTime_element;
    SleepTime_element.data = &SleepTime_data;
    Create_element(&SleepTime_Item, &SleepTime_element);
    
    static ui_data_t AutoSleep_data;
    AutoSleep_data.name = "Auto Sleep Enable";
    AutoSleep_data.ptr = &AutoSleepEnable;
    AutoSleep_data.dataType = UI_DATA_SWITCH;
    AutoSleep_data.actionType = UI_DATA_ACTION_RW;
    static ui_element_t AutoSleep_element;
    AutoSleep_element.data = &AutoSleep_data;
    Create_element(&AutoSleep_Item, &AutoSleep_element);
    
    
    static ui_data_t Year_data;
    Year_data.name = "Year";
    Year_data.ptr = &sTime.Year;
    Year_data.function = SetTime;
    Year_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Year_data.dataType = UI_DATA_INT;
    Year_data.actionType = UI_DATA_ACTION_RW;
    Year_data.max = 99;
    Year_data.min = 0;
    Year_data.step = 1;
    static ui_element_t Year_element;
    Year_element.data = &Year_data;
    Create_element(&Year_Item, &Year_element);
    
    static ui_data_t Month_data;
    Month_data.name = "Month";
    Month_data.ptr = &sTime.Month;
    Month_data.function = SetTime;
    Month_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Month_data.dataType = UI_DATA_INT;
    Month_data.actionType = UI_DATA_ACTION_RW;
    Month_data.max = 12;
    Month_data.min = 1;
    Month_data.step = 1;
    static ui_element_t Month_element;
    Month_element.data = &Month_data;
    Create_element(&Month_Item, &Month_element);
    
    static ui_data_t Days_data;
    Days_data.name = "Date";
    Days_data.ptr = &sTime.Date;
    Days_data.function = SetTime;
    Days_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Days_data.dataType = UI_DATA_INT;
    Days_data.actionType = UI_DATA_ACTION_RW;
    Days_data.max = 31;
    Days_data.min = 1;
    Days_data.step = 1;
    static ui_element_t Days_element;
    Days_element.data = &Days_data;
    Create_element(&Days_Item, &Days_element);
    
    static ui_data_t Hour_data;
    Hour_data.name = "Hour";
    Hour_data.ptr = &sTime.Hours;
    Hour_data.function = SetTime;
    Hour_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Hour_data.dataType = UI_DATA_INT;
    Hour_data.actionType = UI_DATA_ACTION_RW;
    Hour_data.max = 23;
    Hour_data.min = 0;
    Hour_data.step = 1;
    static ui_element_t Hour_element;
    Hour_element.data = &Hour_data;
    Create_element(&Hour_Item, &Hour_element);
    
    static ui_data_t Minute_data;
    Minute_data.name = "Minute";
    Minute_data.ptr = &sTime.Minutes;
    Minute_data.function = SetTime;
    Minute_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Minute_data.dataType = UI_DATA_INT;
    Minute_data.actionType = UI_DATA_ACTION_RW;
    Minute_data.max = 59;
    Minute_data.min = 0;
    Minute_data.step = 1;
    static ui_element_t Minute_element;
    Minute_element.data = &Minute_data;
    Create_element(&Minute_Item, &Minute_element);
    
    static ui_data_t Second_data;
    Second_data.name = "Second";
    Second_data.ptr = &sTime.Seconds;
    Second_data.function = SetTime;
    Second_data.functionType = UI_DATA_FUNCTION_STEP_EXECUTE;
    Second_data.dataType = UI_DATA_INT;
    Second_data.actionType = UI_DATA_ACTION_RW;
    Second_data.max = 59;
    Second_data.min = 0;
    Second_data.step = 1;
    static ui_element_t Second_element;
    Second_element.data = &Second_data;
    Create_element(&Second_Item, &Second_element);
   
   
    static ui_data_t NotificationTime_data;
    NotificationTime_data.name = "Noti Time";
    NotificationTime_data.ptr = &notification_showtime;
    NotificationTime_data.dataType = UI_DATA_INT;
    NotificationTime_data.actionType = UI_DATA_ACTION_RW;
    NotificationTime_data.max = 5000;
    NotificationTime_data.min = 500;
    NotificationTime_data.step = 500;
    static ui_element_t NotificationTime_element;
    NotificationTime_element.data = &NotificationTime_data;
    Create_element(&NotificationTime_Item, &NotificationTime_element);
    
    static ui_data_t WakeInactiveTime_data;
    WakeInactiveTime_data.name = "WkupInacTime";
    WakeInactiveTime_data.ptr = &WakePendingTime;
    WakeInactiveTime_data.dataType = UI_DATA_INT;
    WakeInactiveTime_data.actionType = UI_DATA_ACTION_RW;
    WakeInactiveTime_data.max = 1000;
    WakeInactiveTime_data.min = 100;
    WakeInactiveTime_data.step = 100;
    static ui_element_t WakeInactiveTime_element;
    WakeInactiveTime_element.data = &WakeInactiveTime_data;
    Create_element(&WakeInactiveTime_Item, &WakeInactiveTime_element);
    
    static int Reset_Trigger = 0;
    static ui_data_t Reset_data;
    Reset_data.name = "System Reset";
    Reset_data.ptr = &Reset_Trigger;
    Reset_data.function = Manual_Reset;
    Reset_data.dataType = UI_DATA_SWITCH;
    Reset_data.actionType = UI_DATA_ACTION_RW;
    static ui_element_t Reset_element;
    Reset_element.data = &Reset_data;
    Create_element(&Reset_Item, &Reset_element);
    
    //数据区
    extern float Vbattery;
    static ui_data_t VBAT_data;
    VBAT_data.name = "Battery Voltage";
    VBAT_data.ptr = &Vbattery;
    VBAT_data.dataType = UI_DATA_FLOAT;
    VBAT_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t VBAT_element;
    VBAT_element.data = &VBAT_data;
    Create_element(&VBAT_Item, &VBAT_element);
    
    extern float Icharge;
    static ui_data_t Icharge_data;
    Icharge_data.name = "Charge Current";
    Icharge_data.ptr = &Icharge;
    Icharge_data.dataType = UI_DATA_FLOAT;
    Icharge_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t Icharge_element;
    Icharge_element.data = &Icharge_data;
    Create_element(&ICharge_Item, &Icharge_element);
    
    extern float Tjunction;
    static ui_data_t TJ_data;
    TJ_data.name = "Junction Temp";
    TJ_data.ptr = &Tjunction;
    TJ_data.dataType = UI_DATA_FLOAT;
    TJ_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t TJ_element;
    TJ_element.data = &TJ_data;
    Create_element(&Tj_Item, &TJ_element);
    
    extern float BMP180_AirTemprature;
    static ui_data_t Tair_data;
    Tair_data.name = "Air Temprature";
    Tair_data.ptr = &BMP180_AirTemprature;
    Tair_data.dataType = UI_DATA_FLOAT;
    Tair_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t Tair_element;
    Tair_element.data = &Tair_data;
    Create_element(&Tair_Item, &Tair_element);
    
    extern float BMP180_AirPressure;
    static ui_data_t Pair_data;
    Pair_data.name = "Air Pressure";
    Pair_data.ptr = &BMP180_AirPressure;
    Pair_data.dataType = UI_DATA_INT;
    Pair_data.actionType = UI_DATA_ACTION_RO;
    static ui_element_t Pair_element;
    Pair_element.data = &Pair_data;
    Create_element(&Pair_Item, &Pair_element);
}

/**
 * 在此建立所需显示或更改的文本
 * 无参数
 * 无返回值
 */
void Create_Text(ui_t *ui)
{
    static ui_text_t github_text;
    github_text.font = UI_FONT;
    github_text.fontHight = UI_FONT_HIGHT;
    github_text.fontWidth = UI_FONT_WIDTH;
    github_text.ptr = "Collar FM Receiver\nGit:github.com/TKWTL/Collar-Radio\nUI Provider:github.com/JFeng-Z/MiaoUI";
    static ui_element_t github_element;
    github_element.text = &github_text;
    Create_element(&Github_Item, &github_element);
}

/*
 * 菜单构建函数
 * 该函数不接受参数，也不返回任何值。
 * 功能：静态地构建一个菜单系统。
 */
/**
 * 添加项目到页面
 * 
 * 该函数用于向指定的页面添加一个新的项目。项目包括名称、类型、元素、图像等信息，
 * 并且可以设置项目的功能回调。添加的项目将按照链表的形式管理，同时维护页面上项目的数量。
 *
 * @param name 项目名称，不可为NULL。
 * @param type 项目类型，如数据、开关、文本等。
 * @param Element 项目关联的元素，根据项目类型不同，包含不同的数据。
 * @param image 项目显示的图像，如果为NULL，则使用预设图像。
 * @param item 待添加的项目结构体。
 * @param localPage 项目所在的页面结构体。
 * @param nextPage 项目跳转的下一个页面结构体，如果无跳转，则为NULL。
 * @param function 项目对应的函数指针，用于处理项目特定的功能。
 */
/**
 * @brief 向页面中添加一个新页面。
 * 
 * @param name 新页面的名称。
 * @param page 指向新页面结构体的指针。
 * @param type 新页面的类型。
 */
void Create_MenuTree(ui_t *ui)
{
    AddPage("[RadioPage]", &Menu_Page, UI_PAGE_ICON, NULL);
    
        AddItem("-FM Radio", UI_ITEM_ONCE_FUNCTION, img_radio, &HomeHead_Item, &Menu_Page, NULL, Radio_Run);
    
        AddItem("-Settings", UI_ITEM_PARENTS, img_configuration, &Setting_Item, &Menu_Page, &Setting_Page, NULL);
            AddPage("[Setting]", &Setting_Page, UI_PAGE_ICON, &Menu_Page);
                AddItem("[Menu]", UI_ITEM_RETURN, img_home, &SettingHead_Item, &Setting_Page, &Menu_Page, NULL);
    
                AddItem("-Display", UI_ITEM_PARENTS, img_screen, &Display_Item, &Setting_Page, &Display_Page, NULL);
                    AddPage("[Display]", &Display_Page, UI_PAGE_TEXT, &Setting_Page);
                        AddItem("[Back]", UI_ITEM_RETURN, NULL, &DisplayHead_Item, &Display_Page, &Setting_Page, NULL);
                        AddItem(" Contrast", UI_ITEM_DATA, NULL, &Contrast_Item, &Display_Page, NULL, NULL);
                        AddItem(" BackGround Color", UI_ITEM_DATA, NULL, &MenuColor_Item, &Display_Page, NULL, NULL);
                        AddItem(" AutoSleep Time", UI_ITEM_DATA, NULL, &SleepTime_Item, &Display_Page, NULL, NULL);
                        AddItem(" AutoSleep Enable", UI_ITEM_DATA, NULL, &AutoSleep_Item, &Display_Page, NULL, NULL);
    
                AddItem("-Time & Date", UI_ITEM_PARENTS, img_date, &Time_Item, &Setting_Page, &Time_Page, NULL);
                    AddPage("[Time]", &Time_Page, UI_PAGE_TEXT, &Setting_Page);
                        AddItem("[Back]", UI_ITEM_RETURN, NULL, &TimeHead_Item, &Time_Page, &Setting_Page, NULL);
                        AddItem(" Second", UI_ITEM_DATA, NULL, &Second_Item, &Time_Page, NULL, NULL);
                        AddItem(" Minute", UI_ITEM_DATA, NULL, &Minute_Item, &Time_Page, NULL, NULL);
                        AddItem(" Hour", UI_ITEM_DATA, NULL, &Hour_Item, &Time_Page, NULL, NULL);
                        AddItem(" Date", UI_ITEM_DATA, NULL, &Days_Item, &Time_Page, NULL, NULL);
                        AddItem(" Month", UI_ITEM_DATA, NULL, &Month_Item, &Time_Page, NULL, NULL);
                        AddItem(" Year", UI_ITEM_DATA, NULL, &Year_Item, &Time_Page, NULL, NULL);
                        
                AddItem("-System", UI_ITEM_PARENTS, img_configuration, &SystemSetting_Item, &Setting_Page, &SystemSetting_Page, NULL);
                    AddPage("[System]", &SystemSetting_Page, UI_PAGE_TEXT, &Setting_Page);
                        AddItem("[Back]", UI_ITEM_RETURN, NULL, &SystemHead_Item, &SystemSetting_Page, &Setting_Page, NULL);
                        AddItem(" Notification ShowTime", UI_ITEM_DATA, NULL, &NotificationTime_Item, &SystemSetting_Page, NULL, NULL);
                        AddItem(" Wakeup Inactive Time", UI_ITEM_DATA, NULL, &WakeInactiveTime_Item, &SystemSetting_Page, NULL, NULL);
                        AddItem(" System Reset", UI_ITEM_DATA, NULL, &Reset_Item, &SystemSetting_Page, NULL, NULL);
    
        AddItem("-Statistics", UI_ITEM_PARENTS, img_statistics, &Statistic_Item, &Menu_Page, &Stat_Page, NULL);
            AddPage("[Statistics]", &Stat_Page, UI_PAGE_TEXT, &Menu_Page);
                AddItem("[Menu]", UI_ITEM_RETURN, NULL, &StatHead_Item, &Stat_Page, &Menu_Page, NULL);
                AddItem(" Battery Voltage", UI_ITEM_DATA, NULL, &VBAT_Item, &Stat_Page, NULL, NULL);
                AddItem(" Charge Current", UI_ITEM_DATA, NULL, &ICharge_Item, &Stat_Page, NULL, NULL);
                AddItem(" Air Temprature", UI_ITEM_DATA, NULL, &Tair_Item, &Stat_Page, NULL, NULL);
                AddItem(" Air Pressure", UI_ITEM_DATA, NULL, &Pair_Item, &Stat_Page, NULL, NULL);
                AddItem(" CPU Temprature", UI_ITEM_DATA, NULL, &Tj_Item, &Stat_Page, NULL, NULL);
                
        AddItem("-Tools", UI_ITEM_PARENTS, img_tools, &Tool_Item, &Menu_Page, &Tool_Page, NULL);
            AddPage("[ToolPage]", &Tool_Page, UI_PAGE_ICON, &Menu_Page);
                AddItem("[Menu]", UI_ITEM_RETURN, img_home, &ToolHead_Item, &Tool_Page, &Menu_Page, NULL);
                AddItem("-Gradienter", UI_ITEM_ONCE_FUNCTION, img_gradienter, &Gradienter_Item, &Tool_Page, NULL, Gradienter);
                AddItem("-I2C Scanner", UI_ITEM_ONCE_FUNCTION, img_scan, &I2C_Scanner_Item, &Tool_Page, NULL, i2c_Scanner);
                AddItem("-Burn-in Test", UI_ITEM_ONCE_FUNCTION, img_burn_in, &Burn_in_Test_Item, &Tool_Page, NULL, Burn_in_Tester);
    
        AddItem("-Games", UI_ITEM_PARENTS, img_games, &Game_Item, &Menu_Page, &Game_Page, NULL);
            AddPage("[GamePage]", &Game_Page, UI_PAGE_ICON, &Menu_Page);
                AddItem("[Menu]", UI_ITEM_RETURN, img_home, &GameHead_Item, &Game_Page, &Menu_Page, NULL);
                AddItem("-Game Tetris", UI_ITEM_ONCE_FUNCTION, img_tetris, &Tetris_Item, &Game_Page, NULL, NULL);
                AddItem("-Game Snake", UI_ITEM_ONCE_FUNCTION, img_snake, &Snake_Item, &Game_Page, NULL, Game_Snake);
                AddItem("-Game Dinosaur", UI_ITEM_ONCE_FUNCTION, img_dinosuar, &Dino_Item, &Game_Page, NULL, NULL);
                
        AddItem("-About", UI_ITEM_WORD, img_toby_fox, &Github_Item, &Menu_Page, NULL, NULL);
}

void MiaoUi_Setup(ui_t *ui)
{
    Create_UI(ui, &HomeHead_Item); // 创建UI, 必须给定一个头项目
    RDA5807_Init();
    Radio_Run(ui);
}
