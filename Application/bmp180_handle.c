/*
 *
 *
 *
 */
#include "application.h"

void AirPressure_Task(void *argument)
{
    BMP180_Init();
    for(;;)
    {
        BMP180_StartConv(0x2E);
        osDelay(pdMS_TO_TICKS(5));
        BMP180_GetTemprature();
        osDelay(pdMS_TO_TICKS(500));
        BMP180_StartConv(0xF4);
        osDelay(pdMS_TO_TICKS(26));
        BMP180_GetPressure();
        osDelay(pdMS_TO_TICKS(500));
    }
}