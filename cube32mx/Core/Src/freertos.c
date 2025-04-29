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
#include "app.h"
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
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskEncoder */
osThreadId_t taskEncoderHandle;
const osThreadAttr_t taskEncoder_attributes = {
  .name = "taskEncoder",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskPump */
osThreadId_t taskPumpHandle;
const osThreadAttr_t taskPump_attributes = {
  .name = "taskPump",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskServo */
osThreadId_t taskServoHandle;
const osThreadAttr_t taskServo_attributes = {
  .name = "taskServo",
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
/* Definitions for taskGlassLeds */
osThreadId_t taskGlassLedsHandle;
const osThreadAttr_t taskGlassLeds_attributes = {
  .name = "taskGlassLeds",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskGlassBtns */
osThreadId_t taskGlassBtnsHandle;
const osThreadAttr_t taskGlassBtns_attributes = {
  .name = "taskGlassBtns",
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
/* Definitions for queueManager */
osMessageQueueId_t queueManagerHandle;
const osMessageQueueAttr_t queueManager_attributes = {
  .name = "queueManager"
};
/* Definitions for queuePump */
osMessageQueueId_t queuePumpHandle;
const osMessageQueueAttr_t queuePump_attributes = {
  .name = "queuePump"
};
/* Definitions for queueServo */
osMessageQueueId_t queueServoHandle;
const osMessageQueueAttr_t queueServo_attributes = {
  .name = "queueServo"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskEncoder(void *argument);
void StartTaskPump(void *argument);
void StartTaskServo(void *argument);
void StartTaskDisplay(void *argument);
void StartTaskGladsLeds(void *argument);
void StartTaskGlassBtns(void *argument);
void StartTaskManager(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* creation of queuePump */
  queuePumpHandle = osMessageQueueNew (1, sizeof(uint8_t), &queuePump_attributes);

  /* creation of queueServo */
  queueServoHandle = osMessageQueueNew (1, sizeof(uint8_t), &queueServo_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskEncoder */
  taskEncoderHandle = osThreadNew(StartTaskEncoder, NULL, &taskEncoder_attributes);

  /* creation of taskPump */
  taskPumpHandle = osThreadNew(StartTaskPump, NULL, &taskPump_attributes);

  /* creation of taskServo */
  taskServoHandle = osThreadNew(StartTaskServo, NULL, &taskServo_attributes);

  /* creation of taskDisplay */
  taskDisplayHandle = osThreadNew(StartTaskDisplay, NULL, &taskDisplay_attributes);

  /* creation of taskGlassLeds */
  taskGlassLedsHandle = osThreadNew(StartTaskGladsLeds, NULL, &taskGlassLeds_attributes);

  /* creation of taskGlassBtns */
  taskGlassBtnsHandle = osThreadNew(StartTaskGlassBtns, NULL, &taskGlassBtns_attributes);

  /* creation of taskManager */
  taskManagerHandle = osThreadNew(StartTaskManager, NULL, &taskManager_attributes);

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

/* USER CODE BEGIN Header_StartTaskPump */
/**
 * @brief Function implementing the taskPump thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskPump */
void StartTaskPump(void *argument)
{
  /* USER CODE BEGIN StartTaskPump */
  TaskPump(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskPump */
}

/* USER CODE BEGIN Header_StartTaskServo */
/**
 * @brief Function implementing the taskServo thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskServo */
void StartTaskServo(void *argument)
{
  /* USER CODE BEGIN StartTaskServo */
  TaskServo(argument);
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskServo */
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

/* USER CODE BEGIN Header_StartTaskGladsLeds */
/**
 * @brief Function implementing the taskGlassLeds thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskGladsLeds */
void StartTaskGladsLeds(void *argument)
{
  /* USER CODE BEGIN StartTaskGladsLeds */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskGladsLeds */
}

/* USER CODE BEGIN Header_StartTaskGlassBtns */
/**
 * @brief Function implementing the taskGlassBtns thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskGlassBtns */
void StartTaskGlassBtns(void *argument)
{
  /* USER CODE BEGIN StartTaskGlassBtns */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END StartTaskGlassBtns */
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

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

