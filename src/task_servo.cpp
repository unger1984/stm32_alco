#include "task_servo.h"
#include "app_shared.h"
#include "servo.h"

#include <cmsis_os.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim3;

static Servo servo(&htim3, TIM_CHANNEL_4);

void taskServo_Init(void) {
  // Инициализация сервопривода
  servo.Init();
  servo.SetAngle(0); // Установка начального угла в 0 градусов
}

void taskServo_Run(void) {
  osEventFlagsWait(updateEventHandle, 0x02, osFlagsWaitAll, portMAX_DELAY);
  osStatus_t status = osSemaphoreAcquire(appStateMutexHandle, osWaitForever);
  if (status == osOK) {
    servo.SetAngle(appState.servoAngle);
    osSemaphoreRelease(appStateMutexHandle);
  }
  // osDelay(20);
}