#include "task_servo.h"
#include "servo.h"

#include <cmsis_os.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim3;

static Servo servo(&htim3, TIM_CHANNEL_4);

void taskServo_Init(void) {
  // Инициализация сервопривода
  servo.Init();
  servo.SetAngle(0); // Установка начального угла в 90 градусов
}

void taskServo_Run(void) {
  printf("Servo angle: 0\r\n");
  servo.SetAngle(0);
  osDelay(2000);
  printf("Servo angle: 30\r\n");
  servo.SetAngle(30);
  osDelay(2000);
  printf("Servo angle: 60\r\n");
  servo.SetAngle(60);
  osDelay(2000);
  printf("Servo angle: 90\r\n");
  servo.SetAngle(90);
  osDelay(2000);
  printf("Servo angle: 120\r\n");
  servo.SetAngle(120);
  osDelay(2000);
  printf("Servo angle: 150\r\n");
  servo.SetAngle(150);
  osDelay(2000);
  printf("Servo angle: 180\r\n");
  servo.SetAngle(180);
  osDelay(2000);
}