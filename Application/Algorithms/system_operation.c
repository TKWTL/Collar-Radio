#include "algorithm.h"
#include "image.h"

/***********************以下函数须在ui_conf.c中调用****************************/
ui_item_t SleepTime_Item, AutoSleep_Item;
ui_item_t SystemSetting_Item, SystemHead_Item, NotificationTime_Item, WakeInactiveTime_Item, Reset_Item;
//创建系统设置模块所需的参数
void Create_SystemCtrl_Parameters(ui_t *ui){
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
}

ui_page_t SystemSetting_Page;
//将系统设置模块的对象添加到菜单中
void Add_SystemCtrl_Items(ui_page_t *ParentPage){
    AddItem("-System", UI_ITEM_PARENTS, img_configuration, &SystemSetting_Item, ParentPage, &SystemSetting_Page, NULL);
        AddPage("[System]", &SystemSetting_Page, UI_PAGE_TEXT, ParentPage);
            AddItem("[Back]", UI_ITEM_RETURN, NULL, &SystemHead_Item, &SystemSetting_Page, ParentPage, NULL);
            AddItem(" AutoSleep Time", UI_ITEM_DATA, NULL, &SleepTime_Item, &SystemSetting_Page, NULL, NULL);
            AddItem(" AutoSleep Enable", UI_ITEM_DATA, NULL, &AutoSleep_Item, &SystemSetting_Page, NULL, NULL);
            AddItem(" Notification ShowTime", UI_ITEM_DATA, NULL, &NotificationTime_Item, &SystemSetting_Page, NULL, NULL);
            AddItem(" Wakeup Inactive Time", UI_ITEM_DATA, NULL, &WakeInactiveTime_Item, &SystemSetting_Page, NULL, NULL);
            AddItem(" System Reset", UI_ITEM_DATA, NULL, &Reset_Item, &SystemSetting_Page, NULL, NULL);
}

//系统复位函数
void Manual_Reset(ui_t *ui){
    Disp_ClearBuffer();
    Disp_SendBuffer();
    NVIC_SystemReset();
}

uint8_t IdleDimmed;//自动亮度减半标志位

typedef enum{
    WAKE_NORMAL,
    WAKE_RTC
}WakeReason_t;
WakeReason_t WakeReason = WAKE_NORMAL;

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc){
    WakeReason = WAKE_RTC;
}

/*
 *在无操作时间达到设定值的80%时，屏幕亮度会减半
 *到达设定时间时，系统关闭，关闭OLED，MCU进入stop模式
 *但FM芯片会根据设置来决定是否一同关闭
 *开启的情况下，系统会利用RTC每隔60s唤醒自己检查一次电池电量，避免FM芯片导致的过放电
 */
volatile uint32_t idle_time;//无操作时间，单位10ms
void UserOperationDetected(void){
    idle_time = 0;
    IdleDimmed = 0;
    WakeReason = WAKE_NORMAL;
}


int AutoSleepEnable = 1;//自动休眠设置，默认打开
int Sleep_time = 30;//无操作休眠时间，单位秒

int WakePendingTime = 450;//表示系统唤醒后的不响应时间，单位ms（最好大于ADC采样间隔）
uint8_t Unstable_of_Wake = 0;//表示系统由于唤醒产生的不稳定状态

void SystemClock_Config(void);//main.c中由CubeMX自动生成的时钟配置函数声明

void SystemSleep(void){
    uint8_t RDA5807_Sleep;
    if(idle_time > Sleep_time* (1000/ 10) && (AutoSleepEnable || Vbattery < 3.1f)){//处理无操作关机
        Disp_SetPowerSave(1);
        SC7A20_SetLowPower();
        if(RadioPowerMode == RADIO_OFF || (RadioPowerMode == RADIO_AUTO && LL_GPIO_IsInputPinSet(INDET_GPIO_Port, INDET_Pin)) || Vbattery < 3.1f){
            RDA5807_Sleep = 1;
            RDA5807_PowerOff();
        }
        else{//系统睡眠但RDA5807仍打开时，启动定时唤醒
            RDA5807_Sleep = 0;
            HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 30- 1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
        }
        Unstable_of_Wake = 1;
        
        vTaskSuspendAll();                                                  //挂起其他线程，任何需要异步进行的IO操作都应在此步之前完成
        HAL_Delay(100);
        ADC->CCR &= ~(ADC_CCR_VREFEN| ADC_CCR_TSEN);                        //关闭基准电压和温度传感器以降低功耗
        HAL_SuspendTick();                                                  //HAL库的时基必须也是SysTick，否则停止Tick时无法休眠
        
        HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
        
        SystemClock_Config();                                               //唤醒后必须重新配置时钟
        ADC->CCR |= (ADC_CCR_VREFEN| ADC_CCR_TSEN);                         //恢复基准电压和温度传感器
        xTaskResumeAll();                                                   //恢复线程
        HAL_ResumeTick();                                                   //恢复tick必须在恢复线程后进行，否则立刻触发调度后会发现所有任务都被阻塞
        
        if(RDA5807_Sleep == 0) HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);
        else RDA5807_PowerOn();
        
        if(WakeReason == WAKE_RTC){
            LL_GPIO_SetOutputPin(CPEN_GPIO_Port, CPEN_Pin);//开启分压器
            osDelay(pdMS_TO_TICKS(WakePendingTime));//延时避免误触发
            Unstable_of_Wake = 0;
            osDelay(pdMS_TO_TICKS(WakePendingTime));//延时，ADC线程检测电池电压，如果检测到低电压事件，系统会直接唤醒
            idle_time = Sleep_time* (1000/ 10)+ 1;//在下一个循环直接进入睡眠
        }
        if(WakeReason != WAKE_RTC){//不使用else是为了在检测过程中也可以正确唤醒
            Disp_SetContrast2(Contrast);
            Disp_SetPowerSave(0);
            SC7A20_Init();
            
            osDelay(pdMS_TO_TICKS(WakePendingTime));//延时避免误触发
            Unstable_of_Wake = 0;
        
            idle_time = WakePendingTime;
        }
    }
    else if(idle_time > Sleep_time* 80){//处理无操作暗屏
        if(IdleDimmed == 0){
            IdleDimmed = 1;
            Disp_SetContrast2(Contrast == 1? Contrast : Contrast/ 2);
        }
    }
    else{//恢复屏幕亮度
        if(IdleDimmed == 0 && idle_time == 0){
            Disp_SetContrast2(Contrast);
        }
    }
    idle_time++;
    
    osDelay(pdMS_TO_TICKS(10));
}
