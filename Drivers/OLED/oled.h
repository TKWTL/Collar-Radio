#ifndef __OLED_H__
#define __OLED_H__

//#include "stdio.h"
#include "main.h"
#include "i2c.h"
#include "u8g2.h"
#include "string.h"

#define OLED_ADDRESS    0x78

uint8_t u8x8_gpio_and_delay_hw(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif
