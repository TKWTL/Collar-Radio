/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : app_freertos.c
  * Description        : Code for freertos applications
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
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for SysShutdown */
osThreadId_t SysShutdownHandle;
uint32_t SysShutdownBuffer[ 128 ];
osStaticThreadDef_t SysShutdownControlBlock;
const osThreadAttr_t SysShutdown_attributes = {
  .name = "SysShutdown",
  .stack_mem = &SysShutdownBuffer[0],
  .stack_size = sizeof(SysShutdownBuffer),
  .cb_mem = &SysShutdownControlBlock,
  .cb_size = sizeof(SysShutdownControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Menu */
osThreadId_t MenuHandle;
uint32_t MenuBuffer[ 512 ];
osStaticThreadDef_t MenuControlBlock;
const osThreadAttr_t Menu_attributes = {
  .name = "Menu",
  .stack_mem = &MenuBuffer[0],
  .stack_size = sizeof(MenuBuffer),
  .cb_mem = &MenuControlBlock,
  .cb_size = sizeof(MenuControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ADC_Update */
osThreadId_t ADC_UpdateHandle;
uint32_t ADC_UpdateBuffer[ 64 ];
osStaticThreadDef_t ADC_UpdateControlBlock;
const osThreadAttr_t ADC_Update_attributes = {
  .name = "ADC_Update",
  .stack_mem = &ADC_UpdateBuffer[0],
  .stack_size = sizeof(ADC_UpdateBuffer),
  .cb_mem = &ADC_UpdateControlBlock,
  .cb_size = sizeof(ADC_UpdateControlBlock),
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for PressureSensor */
osThreadId_t PressureSensorHandle;
uint32_t PressureSensorBuffer[ 64 ];
osStaticThreadDef_t PressureSensorControlBlock;
const osThreadAttr_t PressureSensor_attributes = {
  .name = "PressureSensor",
  .stack_mem = &PressureSensorBuffer[0],
  .stack_size = sizeof(PressureSensorBuffer),
  .cb_mem = &PressureSensorControlBlock,
  .cb_size = sizeof(PressureSensorControlBlock),
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for ButtonTask */
osThreadId_t ButtonTaskHandle;
uint32_t ButtonTaskBuffer[ 64 ];
osStaticThreadDef_t ButtonTaskControlBlock;
const osThreadAttr_t ButtonTask_attributes = {
  .name = "ButtonTask",
  .stack_mem = &ButtonTaskBuffer[0],
  .stack_size = sizeof(ButtonTaskBuffer),
  .cb_mem = &ButtonTaskControlBlock,
  .cb_size = sizeof(ButtonTaskControlBlock),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Notification */
osThreadId_t NotificationHandle;
uint32_t NotificationBuffer[ 128 ];
osStaticThreadDef_t NotificationControlBlock;
const osThreadAttr_t Notification_attributes = {
  .name = "Notification",
  .stack_mem = &NotificationBuffer[0],
  .stack_size = sizeof(NotificationBuffer),
  .cb_mem = &NotificationControlBlock,
  .cb_size = sizeof(NotificationControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SC7A20 */
osThreadId_t SC7A20Handle;
uint32_t SC7A20Buffer[ 64 ];
osStaticThreadDef_t SC7A20ControlBlock;
const osThreadAttr_t SC7A20_attributes = {
  .name = "SC7A20",
  .stack_mem = &SC7A20Buffer[0],
  .stack_size = sizeof(SC7A20Buffer),
  .cb_mem = &SC7A20ControlBlock,
  .cb_size = sizeof(SC7A20ControlBlock),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Button_Queue */
osMessageQueueId_t Button_QueueHandle;
uint8_t Button_QueueBuffer[ 32 * sizeof( uint16_t ) ];
osStaticMessageQDef_t Button_QueueControlBlock;
const osMessageQueueAttr_t Button_Queue_attributes = {
  .name = "Button_Queue",
  .cb_mem = &Button_QueueControlBlock,
  .cb_size = sizeof(Button_QueueControlBlock),
  .mq_mem = &Button_QueueBuffer,
  .mq_size = sizeof(Button_QueueBuffer)
};
/* Definitions for Notification_Queue */
osMessageQueueId_t Notification_QueueHandle;
uint8_t NotificationQueueBuffer[ 16 * sizeof( const char* ) ];
osStaticMessageQDef_t NotificationQueueControlBlock;
const osMessageQueueAttr_t Notification_Queue_attributes = {
  .name = "Notification_Queue",
  .cb_mem = &NotificationQueueControlBlock,
  .cb_size = sizeof(NotificationQueueControlBlock),
  .mq_mem = &NotificationQueueBuffer,
  .mq_size = sizeof(NotificationQueueBuffer)
};
/* Definitions for mutex_i2c_idle */
osSemaphoreId_t mutex_i2c_idleHandle;
osStaticSemaphoreDef_t mutex_i2c_idleControlBlock;
const osSemaphoreAttr_t mutex_i2c_idle_attributes = {
  .name = "mutex_i2c_idle",
  .cb_mem = &mutex_i2c_idleControlBlock,
  .cb_size = sizeof(mutex_i2c_idleControlBlock),
};
/* Definitions for mutex_i2c_busy */
osSemaphoreId_t mutex_i2c_busyHandle;
osStaticSemaphoreDef_t mutex_i2c_busyControlBlock;
const osSemaphoreAttr_t mutex_i2c_busy_attributes = {
  .name = "mutex_i2c_busy",
  .cb_mem = &mutex_i2c_busyControlBlock,
  .cb_size = sizeof(mutex_i2c_busyControlBlock),
};
/* Definitions for mutex_disp_idle */
osSemaphoreId_t mutex_disp_idleHandle;
osStaticSemaphoreDef_t mutex_disp_idleControlBlock;
const osSemaphoreAttr_t mutex_disp_idle_attributes = {
  .name = "mutex_disp_idle",
  .cb_mem = &mutex_disp_idleControlBlock,
  .cb_size = sizeof(mutex_disp_idleControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void SysShutdown_Task(void *argument);
void Menu_Task(void *argument);
void ADC_Task(void *argument);
void AirPressure_Task(void *argument);
void Button_Handle(void *argument);
void Notification_Task(void *argument);
void SC7A20_Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
//空闲任务钩子函数
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
    __WFI();//这里经过修改，加入了休眠指令，以提高电源效率
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of mutex_i2c_idle */
  mutex_i2c_idleHandle = osSemaphoreNew(1, 1, &mutex_i2c_idle_attributes);

  /* creation of mutex_i2c_busy */
  mutex_i2c_busyHandle = osSemaphoreNew(1, 0, &mutex_i2c_busy_attributes);

  /* creation of mutex_disp_idle */
  mutex_disp_idleHandle = osSemaphoreNew(1, 1, &mutex_disp_idle_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Button_Queue */
  Button_QueueHandle = osMessageQueueNew (32, sizeof(uint16_t), &Button_Queue_attributes);

  /* creation of Notification_Queue */
  Notification_QueueHandle = osMessageQueueNew (16, sizeof(const char*), &Notification_Queue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of SysShutdown */
  SysShutdownHandle = osThreadNew(SysShutdown_Task, NULL, &SysShutdown_attributes);

  /* creation of Menu */
  MenuHandle = osThreadNew(Menu_Task, NULL, &Menu_attributes);

  /* creation of ADC_Update */
  ADC_UpdateHandle = osThreadNew(ADC_Task, NULL, &ADC_Update_attributes);

  /* creation of PressureSensor */
  PressureSensorHandle = osThreadNew(AirPressure_Task, NULL, &PressureSensor_attributes);

  /* creation of ButtonTask */
  ButtonTaskHandle = osThreadNew(Button_Handle, NULL, &ButtonTask_attributes);

  /* creation of Notification */
  NotificationHandle = osThreadNew(Notification_Task, NULL, &Notification_attributes);

  /* creation of SC7A20 */
  SC7A20Handle = osThreadNew(SC7A20_Task, NULL, &SC7A20_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_SysShutdown_Task */
/**
  * @brief  Function implementing the SysShutdown thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_SysShutdown_Task */
__weak void SysShutdown_Task(void *argument)
{
  /* USER CODE BEGIN SysShutdown_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SysShutdown_Task */
}

/* USER CODE BEGIN Header_Menu_Task */
/**
* @brief Function implementing the Menu thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Menu_Task */
__weak void Menu_Task(void *argument)
{
  /* USER CODE BEGIN Menu_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
    
  /* USER CODE END Menu_Task */
}

/* USER CODE BEGIN Header_ADC_Task */
/**
* @brief Function implementing the ADC_Update thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ADC_Task */
__weak void ADC_Task(void *argument)
{
  /* USER CODE BEGIN ADC_Task */
  /* Infinite loop */
    for(;;)
    {
    }
  /* USER CODE END ADC_Task */
}

/* USER CODE BEGIN Header_AirPressure_Task */
/**
* @brief Function implementing the PressureSensor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AirPressure_Task */
__weak void AirPressure_Task(void *argument)
{
  /* USER CODE BEGIN AirPressure_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END AirPressure_Task */
}

/* USER CODE BEGIN Header_Button_Handle */
/**
* @brief Function implementing the ButtonTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Button_Handle */
__weak void Button_Handle(void *argument)
{
  /* USER CODE BEGIN Button_Handle */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Button_Handle */
}

/* USER CODE BEGIN Header_Notification_Task */
/**
* @brief Function implementing the Notification thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Notification_Task */
__weak void Notification_Task(void *argument)
{
  /* USER CODE BEGIN Notification_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Notification_Task */
}

/* USER CODE BEGIN Header_SC7A20_Task */
/**
* @brief Function implementing the SC7A20 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SC7A20_Task */
__weak void SC7A20_Task(void *argument)
{
  /* USER CODE BEGIN SC7A20_Task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END SC7A20_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

