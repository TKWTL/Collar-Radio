
#ifndef __RADIO_H__
#define __RADIO_H__

#ifdef __cplusplus
extern "C" {
#endif
 
#include "application.h"
    
typedef enum{
    MODE_SEARCH,
    MODE_STA,
    MODE_FREQ
}RadioMode_t;

typedef enum {
    RADIO_ON,
    RADIO_AUTO,
    RADIO_OFF
}RadioPowerState_t;

extern RadioPowerState_t RadioPowerMode;

//收音机函数声明，供UI层调用
void Radio_Run(ui_t *ui);
    
#ifdef __cplusplus
}
#endif

#endif
