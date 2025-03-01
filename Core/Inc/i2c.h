/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
  *Encoded with UTF-8
  *I2C基础读写库函数，提供I2C的DMA发送与中断接收
  *依赖于两个信号量实现的互斥锁实现有序无冲突访问I2C资源
  *mutex_i2c_idle初始化为1，开始读取时，等待busy被Take后Give，实现对I2C的互斥访问
  *mutex_i2c_busy初始化为0，ISR回调函数中Give，调用读写函数的进程Take，实现非阻塞式等待
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */
extern osSemaphoreId_t mutex_i2c_idleHandle;
extern osSemaphoreId_t mutex_i2c_busyHandle;
    
void I2C_Transmit(uint8_t addr, uint8_t* p_buf, uint32_t len);
void I2C_Receive(uint8_t addr, uint8_t* p_buf, uint32_t len);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

