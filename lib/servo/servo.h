/**
 * @file servo.h
 * @brief Servo class for handling servo motors
 * @details This class provides methods to initialize, update, and set the
 * position of a servo motor using STM32 timers.
 *
 * @anchor Andrey Unger unger1984@gmail.com
 *
 * Это упрощенный класс для работы сервоприводом 180 градусов.
 * Он использует таймеры STM32 для отслеживания положения.
 *
 * Не проверял портируемость и совместимость!
 */

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