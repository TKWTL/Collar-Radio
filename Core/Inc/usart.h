/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  *Encode with UTF-8
  *基于Cube MX自动生成的代码填充而来，
  *对外提供串口打印函数usart_printf()与usart_puts()
  *内部使用HAL库、DMA加中断加单缓冲实现，
  *如工作不正常，请查看此处定义的USART号，中断号，中断优先级以及DMA通道是否存在冲突
  *参考：
  */
  
//调试用USART定义
#define DEBUG_USART huart1

//发送缓冲区长度
#define BEBUG_TX_BUFFER_LENGTH  192

/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include <stdarg.h>
#include <stdio.h>
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void uart_puts(const char *s);
void uart_printf(const char *format, ...);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

