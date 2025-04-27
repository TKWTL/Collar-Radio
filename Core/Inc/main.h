/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
  *Encoder:UTF-8
  *系统的各种定义位置
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

#include "stm32g0xx_ll_rcc.h"
#include "stm32g0xx_ll_bus.h"
#include "stm32g0xx_ll_system.h"
#include "stm32g0xx_ll_exti.h"
#include "stm32g0xx_ll_cortex.h"
#include "stm32g0xx_ll_utils.h"
#include "stm32g0xx_ll_pwr.h"
#include "stm32g0xx_ll_dma.h"
#include "stm32g0xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//片上外设次级驱动库
#include "i2c.h"
#include "usart.h"
#include "adc.h"
#include "rtc.h"
    
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBAT_Pin LL_GPIO_PIN_2
#define VBAT_GPIO_Port GPIOA
#define F_Pin LL_GPIO_PIN_7
#define F_GPIO_Port GPIOA
#define F_EXTI_IRQn EXTI4_15_IRQn
#define B_Pin LL_GPIO_PIN_0
#define B_GPIO_Port GPIOB
#define B_EXTI_IRQn EXTI0_1_IRQn
#define E_Pin LL_GPIO_PIN_1
#define E_GPIO_Port GPIOB
#define E_EXTI_IRQn EXTI0_1_IRQn
#define ICHG_Pin LL_GPIO_PIN_2
#define ICHG_GPIO_Port GPIOB
#define CHRG_Pin LL_GPIO_PIN_9
#define CHRG_GPIO_Port GPIOA
#define CHRG_EXTI_IRQn EXTI4_15_IRQn
#define STDBY_Pin LL_GPIO_PIN_6
#define STDBY_GPIO_Port GPIOC
#define STDBY_EXTI_IRQn EXTI4_15_IRQn
#define INT_Pin LL_GPIO_PIN_10
#define INT_GPIO_Port GPIOA
#define INT_EXTI_IRQn EXTI4_15_IRQn
#define SCL_Pin LL_GPIO_PIN_11
#define SCL_GPIO_Port GPIOA
#define SDA_Pin LL_GPIO_PIN_12
#define SDA_GPIO_Port GPIOA
#define D_Pin LL_GPIO_PIN_15
#define D_GPIO_Port GPIOA
#define D_EXTI_IRQn EXTI4_15_IRQn
#define C_Pin LL_GPIO_PIN_3
#define C_GPIO_Port GPIOB
#define C_EXTI_IRQn EXTI2_3_IRQn
#define A_Pin LL_GPIO_PIN_4
#define A_GPIO_Port GPIOB
#define A_EXTI_IRQn EXTI4_15_IRQn
#define INDET_Pin LL_GPIO_PIN_5
#define INDET_GPIO_Port GPIOB
#define INDET_EXTI_IRQn EXTI4_15_IRQn
#define TX_Pin LL_GPIO_PIN_6
#define TX_GPIO_Port GPIOB
#define RX_Pin LL_GPIO_PIN_7
#define RX_GPIO_Port GPIOB
#define CPEN_Pin LL_GPIO_PIN_8
#define CPEN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
