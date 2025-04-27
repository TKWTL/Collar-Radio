
#include "application.h"

void ADC_Task(void *argument)
{
    osDelay(pdMS_TO_TICKS(400));
    for(;;)
    {
        ADC_SampleandFilter();
        UpdateChargeState();
    }
}