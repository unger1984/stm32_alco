#pragma once
#include "tim.h"

#define SERVO_MIN 500  // минимум (в микросекундах)
#define SERVO_MAX 2400 // максимум

#ifdef __cplusplus

class Servo {
public:
  Servo(TIM_HandleTypeDef *htim, uint32_t channel)
      : htim_(htim), channel_(channel) {};

  void Init();
  void SetAngle(uint8_t angle);

private:
  TIM_HandleTypeDef *htim_;
  uint32_t channel_;
};

#endif