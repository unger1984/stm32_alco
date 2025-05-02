/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
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
#include "app_shared.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskEncoder */
osThreadId_t taskEncoderHandle;
const osThreadAttr_t taskEncoder_attributes = {
  .name = "taskEncoder",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskDisplay */
osThreadId_t taskDisplayHandle;
const osThreadAttr_t taskDisplay_attributes = {
  .name = "taskDisplay",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskManager */
osThreadId_t taskManagerHandle;
const osThreadAttr_t taskManager_attributes = {
  .name = "taskManager",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskWorker */
osThreadId_t taskWorkerHandle;
const osThreadAttr_t taskWorker_attributes = {
  .name = "taskWorker",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskHardware */
osThreadId_t taskHardwareHandle;
const osThreadAttr_t taskHardware_attributes = {
  .name = "taskHardware",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for queueManager */
osMessageQueueId_t queueManagerHandle;
const osMessageQueueAttr_t queueManager_attributes = {
  .name = "queueManager"
};
/* Definitions for queueWorker */
osMessageQueueId_t queueWorkerHandle;
const osMessageQueueAttr_t queueWorker_attributes = {
  .name = "queueWorker"
};
/* Definitions for queueHardware */
osMessageQueueId_t queueHardwareHandle;
const osMessageQueueAttr_t queueHardware_attributes = {
  .name = "queueHardware"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskEncoder(void *argument);
void StartTaskDisplay(void *argument);
void StartTaskManager(void *argument);
void StartTaskWorker(void *argument);
void StartTaskHardware(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
extern TIM_HandleTypeDef htim2;

void configureTimerForRunTimeStats(void) { HAL_TIM_Base_Start(&htim2); }

uint32_t getRunTimeCounterValue(void) { return __HAL_TIM_GET_COUNTER(&htim2); }
/* USER CODE END 1 */

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

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of queueManager */
  queueManagerHandle = osMessageQueueNew (7, sizeof(ManagerEvent), &queueManager_attributes);

  /* creation of queueWorker */
  queueWorkerHandle = osMessageQueueNew (1, sizeof(WorkerEvent), &queueWorker_attributes);

  /* creation of queueHardware */
  queueHardwareHandle = osMessageQueueNew (1, sizeof(HardwareEvent), &queueHardware_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskEncoder */
  taskEncoderHandle = osThreadNew(StartTaskEncoder, NULL, &taskEncoder_attributes);

  /* creation of taskDisplay */
  taskDisplayHandle = osThreadNew(StartTaskDisplay, NULL, &taskDisplay_attributes);

  /* creation of taskManager */
  taskManagerHandle = osThreadNew(StartTaskManager, NULL, &taskManager_attributes);

  /* creation of taskWorker */
  taskWorkerHandle = osThreadNew(StartTaskWorker, NULL, &taskWorker_attributes);

  /* creation of taskHardware */
  taskHardwareHandle = osThreadNew(StartTaskHardware, NULL, &taskHardware_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTaskEncoder */
/**
 * @brief Function implementing the taskEncoder thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskEncoder */
void StartTaskEncoder(void *argument)
{
  /* USER CODE BEGIN StartTaskEncoder */
  TaskEncoder(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskEncoder */
}

/* USER CODE BEGIN Header_StartTaskDisplay */
/**
 * @brief Function implementing the taskDisplay thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskDisplay */
void StartTaskDisplay(void *argument)
{
  /* USER CODE BEGIN StartTaskDisplay */
  TaskDisplay(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskDisplay */
}

/* USER CODE BEGIN Header_StartTaskManager */
/**
 * @brief Function implementing the taskManager thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskManager */
void StartTaskManager(void *argument)
{
  /* USER CODE BEGIN StartTaskManager */
  TaskManager(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskManager */
}

/* USER CODE BEGIN Header_StartTaskWorker */
/**
 * @brief Function implementing the taskWorker thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskWorker */
void StartTaskWorker(void *argument)
{
  /* USER CODE BEGIN StartTaskWorker */
  TaskWorker(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskWorker */
}

/* USER CODE BEGIN Header_StartTaskHardware */
/**
* @brief Function implementing the taskHardware thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTaskHardware */
void StartTaskHardware(void *argument)
{
  /* USER CODE BEGIN StartTaskHardware */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartTaskHardware */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

