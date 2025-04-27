#include "app.h"
#include "tim.h"
#include <cmsis_os2.h>

#define SERVO_MIN 500  // минимум (в микросекундах)
#define SERVO_MAX 2400 // максимум

void setAngle(uint8_t angle) {
  if (angle > 180)
    angle = 180;
  else if (angle < 0)
    angle = 0;

  uint16_t pulse =
      SERVO_MIN + ((uint32_t)(SERVO_MAX - SERVO_MIN) * angle) / 180;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse);
}

void sendServoStatus(uint8_t angle) {
  ManagerEvent_t event = {
      .source = SERVO,
      .data =
          {
              .angle = angle,
          },
  };
  osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
}

void TaskServo(void *argument) {
  uint8_t current = 0;
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  setAngle(current);
  sendServoStatus(current);

  for (;;) {
    uint8_t cmd;
    osStatus_t wait =
        osMessageQueueGet(queueServoHandle, &cmd, NULL, osWaitForever);
    if (wait == osStatus_t::osOK) {
      if (current != cmd) {
        setAngle(cmd);
        current = cmd;
      }
      sendServoStatus(cmd);
    }
  }
}