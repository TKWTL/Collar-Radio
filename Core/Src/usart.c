/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
  *内部使用HAL库、DMA加中断加双缓冲实现，
  *参考：
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */

  /** Initializes the peripherals clocks
  */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
      Error_Handler();
    }

    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = TX_Pin|RX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
    GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel7;
    hdma_usart1_tx.Init.Request = DMA_REQUEST_USART1_TX;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, TX_Pin|RX_Pin);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//发送缓冲区数组
static uint8_t tx_buffer[BEBUG_TX_BUFFER_LENGTH];
//头指针与数据长度
static uint32_t p_tx_head = 0;
static uint32_t tx_length = 0;
//DMA发送中标志位
static uint8_t tx_transmitting = 0;

//DMA发送函数封装
static void uart_DMATX_Start(){
    tx_transmitting = 1;
    if(p_tx_head + tx_length < BEBUG_TX_BUFFER_LENGTH){//未超过缓冲区尾部
        HAL_UART_Transmit_DMA(&DEBUG_USART, (const uint8_t*)(tx_buffer + p_tx_head), tx_length);
        p_tx_head += tx_length;
        tx_length = 0;
    }
    else{//越过或触及缓冲区尾部，只负责发送到尾部，剩下的通过回调函数重发
        HAL_UART_Transmit_DMA(&DEBUG_USART, (const uint8_t*)(tx_buffer + p_tx_head), BEBUG_TX_BUFFER_LENGTH - p_tx_head);
        tx_length -= BEBUG_TX_BUFFER_LENGTH - p_tx_head;
        p_tx_head = 0;
    }
}

/*将提供的字符串填到发送缓冲区内，遇到'\0'时停止，爆缓冲区了也停止
 *爆缓冲区的标准为DMA未发数据个数+有效数据个数大于缓冲区大小
 *填完后若串口未处于发送状态，则启动DMA发送
*/
void uart_puts(const char *s){
    uint32_t len = 0;
    while(1){
        if(s[len] == 0x00) break;
        if(tx_length + __HAL_DMA_GET_COUNTER(&hdma_usart1_tx) > BEBUG_TX_BUFFER_LENGTH) break;
        
        if(p_tx_head + tx_length < BEBUG_TX_BUFFER_LENGTH){//未超过缓冲区尾部
            tx_buffer[p_tx_head + tx_length] = s[len++];
        }
        else{//越过或触及缓冲区尾部
            tx_buffer[p_tx_head + tx_length - BEBUG_TX_BUFFER_LENGTH] = s[len++];
        }
        tx_length++;
    }
    if(len > 0 && tx_transmitting == 0){
        uart_DMATX_Start();//未发送则启动发送
    }
}

void uart_printf(const char *format, ...){    
    char buf[64];
    va_list arg;
    va_start(arg,format);
    vsprintf(buf,format,arg);
    va_end(arg);
    uart_puts(buf);
}
//DMA发送完成回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if(tx_length == 0){
        tx_transmitting = 0;//发送完成了
    }
    else{
        uart_DMATX_Start();//还有数据要发
    }
}

/* USER CODE END 1 */
