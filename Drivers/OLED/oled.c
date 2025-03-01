/*
 * oled.c
 *
 *  Created on: 2025/02//14
 *      Author: TKWTL
 */

#include "oled.h"

uint8_t u8x8_gpio_and_delay_hw(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
            break;
        case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
            break;
        case U8X8_MSG_DELAY_MILLI: // delay arg_int * 1 milli second
            osDelay(pdMS_TO_TICKS(1));
            break;
        case U8X8_MSG_DELAY_I2C: // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
            break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
            break;                    // arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1); // default return value
            break;
    }
    return 1;
}

volatile uint8_t oledbuf[32];
uint8_t l_oledbuf;
uint8_t *p_oledbuf;
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch(msg) {
        case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;
        
        case U8X8_MSG_BYTE_START_TRANSFER:
            l_oledbuf = 0;
        break;
        
        case U8X8_MSG_BYTE_SEND:
            p_oledbuf = (uint8_t*)arg_ptr;
            while(arg_int){
                oledbuf[l_oledbuf++] = *p_oledbuf;
                p_oledbuf++;
                arg_int--;
            }
        break;
            
        case U8X8_MSG_BYTE_END_TRANSFER:
            I2C_Transmit(OLED_ADDRESS, (uint8_t*)oledbuf, l_oledbuf);
        break;
        
        case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
        break;
        
        default:
            return 0;
    }
    return 1;
}
