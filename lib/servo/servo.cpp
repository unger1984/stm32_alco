#include "servo.h"
#include <stdio.h>

void Servo::Init() {
  // Инициализация сервопривода
  HAL_TIM_PWM_Start(htim_, channel_);
}

void Servo::SetAngle(uint8_t angle) {
  if (angle > 180)
    angle = 180;
  else if (angle < 0)
    angle = 0;

  uint16_t pulse =
      SERVO_MIN + ((uint32_t)(SERVO_MAX - SERVO_MIN) * angle) / 180;
  __HAL_TIM_SET_COMPARE(htim_, channel_, pulse);
}