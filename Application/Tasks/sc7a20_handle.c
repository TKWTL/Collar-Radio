#include "application.h"

void SC7A20_Task(void *argument){
    SC7A20_Init();
    for(;;){
        SC7A20_Outdata_Load();
        osDelay(pdMS_TO_TICKS(125));
    }
}