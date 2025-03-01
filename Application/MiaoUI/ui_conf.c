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
#include "image.h"
#include "custom.h"
#include "version.h"

/*Page*/
ui_page_t   Menu_Page, 
            Setting_Page,
            Stat_Page;
/*item */
ui_item_t HomeHead_Item, Github_Item;

ui_item_t Setting_Item, SettingHead_Item;
ui_item_t Contrast_Item, MenuColor_Item;

ui_item_t Statistic_Item, StatHead_Item;
ui_item_t VBAT_Item, ICharge_Item, Tj_Item, Tair_Item, Pair_Item;

/**
 * 在此建立所需显示或更改的数据
 * 无参数
 * 无返回值
 */
void Create_Parameter(ui_t *ui)
{
    //设置区
    static int Contrast = 15;
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
    github_text.ptr = VERSION_PROJECT_LINK;
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
void Draw_Radio_Page(ui_t *ui);
void Create_MenuTree(ui_t *ui)
{
    AddPage("[RadioPage]", &Menu_Page, UI_PAGE_ICON);
    
        AddItem("-FM Radio", UI_ITEM_ONCE_FUNCTION, logo_allArray[0], &HomeHead_Item, &Menu_Page, NULL, Show_Version);
    
        AddItem("-Setting", UI_ITEM_PARENTS, logo_allArray[1], &Setting_Item, &Menu_Page, &Setting_Page, NULL);
            AddPage("[Setting]", &Setting_Page, UI_PAGE_TEXT);
                AddItem("[Menu]", UI_ITEM_RETURN, NULL, &SettingHead_Item, &Setting_Page, &Menu_Page, NULL);
                AddItem(" Contrast", UI_ITEM_DATA, NULL, &Contrast_Item, &Setting_Page, NULL, NULL);
                AddItem(" BackGround Color", UI_ITEM_DATA, NULL, &MenuColor_Item, &Setting_Page, NULL, NULL);
    
        AddItem("-Statistic", UI_ITEM_PARENTS, logo_allArray[5], &Statistic_Item, &Menu_Page, &Stat_Page, NULL);
            AddPage("[Statistic]", &Stat_Page, UI_PAGE_TEXT);
                AddItem("[Menu]", UI_ITEM_RETURN, NULL, &StatHead_Item, &Stat_Page, &Menu_Page, NULL);
                AddItem(" Battery Voltage", UI_ITEM_DATA, NULL, &VBAT_Item, &Stat_Page, NULL, NULL);
                AddItem(" Charge Current", UI_ITEM_DATA, NULL, &ICharge_Item, &Stat_Page, NULL, NULL);
                AddItem(" Air Temprature", UI_ITEM_DATA, NULL, &Tair_Item, &Stat_Page, NULL, NULL);
                AddItem(" Air Pressure", UI_ITEM_DATA, NULL, &Pair_Item, &Stat_Page, NULL, NULL);
                AddItem(" CPU Temprature", UI_ITEM_DATA, NULL, &Tj_Item, &Stat_Page, NULL, NULL);
    
        //AddItem("-Gradienter", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
        //AddItem("-Game Tetris", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
        //AddItem("-Game Dinosaur", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
        //AddItem("-Game Snake", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
        //AddItem("-Burn-in Test", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
        AddItem("-About", UI_ITEM_WORD, logo_allArray[3], &Github_Item, &Menu_Page, NULL, NULL);
}

void MiaoUi_Setup(ui_t *ui)
{
    Create_UI(ui, &HomeHead_Item); // 创建UI, 必须给定一个头项目
    Show_Version(ui);
}
