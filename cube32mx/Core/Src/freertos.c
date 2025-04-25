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
#include "task_encoder.h"
#include "task_oled.h"
#include "task_servo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
volatile AppState appState;
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
/* Definitions for taskServo */
osThreadId_t taskServoHandle;
const osThreadAttr_t taskServo_attributes = {
  .name = "taskServo",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskOled */
osThreadId_t taskOledHandle;
const osThreadAttr_t taskOled_attributes = {
  .name = "taskOled",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for appStateMutex */
osSemaphoreId_t appStateMutexHandle;
osStaticSemaphoreDef_t menuStateMutexControlBlock;
const osSemaphoreAttr_t appStateMutex_attributes = {
  .name = "appStateMutex",
  .cb_mem = &menuStateMutexControlBlock,
  .cb_size = sizeof(menuStateMutexControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTaskEncoder(void *argument);
void StartTaskServo(void *argument);
void StartTaskOled(void *argument);

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

  /* Create the semaphores(s) */
  /* creation of appStateMutex */
  appStateMutexHandle = osSemaphoreNew(1, 1, &appStateMutex_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of taskEncoder */
  taskEncoderHandle = osThreadNew(StartTaskEncoder, NULL, &taskEncoder_attributes);

  /* creation of taskServo */
  taskServoHandle = osThreadNew(StartTaskServo, NULL, &taskServo_attributes);

  /* creation of taskOled */
  taskOledHandle = osThreadNew(StartTaskOled, NULL, &taskOled_attributes);

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
  taskEncoder_Init();
  /* Infinite loop */
  for (;;) {
    taskEncoder_Run();
    osDelay(1);
  }
  /* USER CODE END StartTaskEncoder */
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
  taskServo_Init();
  /* Infinite loop */
  for (;;) {
    taskServo_Run();
    osDelay(1);
  }
  /* USER CODE END StartTaskServo */
}

/* USER CODE BEGIN Header_StartTaskOled */
/**
 * @brief Function implementing the taskOled thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTaskOled */
void StartTaskOled(void *argument)
{
  /* USER CODE BEGIN StartTaskOled */
  taskOled_Init();
  /* Infinite loop */
  for (;;) {
    taskOled_Run();
    osDelay(1);
  }
  /* USER CODE END StartTaskOled */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

