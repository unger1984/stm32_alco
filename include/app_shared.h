#pragma once

#ifndef _APP_SHARED_H
#define _APP_SHARED_H

#include <cmsis_os.h>
#include <stdint.h>

#define LONG_PRESS_DURATION 500

#ifdef __cplusplus
extern "C" {
#endif

extern osMessageQueueId_t queueManagerHandle;
extern osMessageQueueId_t queuePumpHandle;
extern osMessageQueueId_t queueServoHandle;
extern osMessageQueueId_t queueWorkerHandle;
extern osThreadId_t taskDisplayHandle;

void TaskEncoder(void *argument);
void TaskManager(void *argument);
void TaskPump(void *argument);
void TaskServo(void *argument);
void TaskDisplay(void *argument);
void TaskWorker(void *argument);

/// @brief Тип события энкодера
typedef enum EncoderEventType {
  NONE = 0, // нет событий
  ROTATE,   // вращение
  PRESS,    // Начало нажатия кнопки
  RELEASE,  // Отпускание кнопки
  HOLD,     // Удержание кнопки
} EncoderEventType;

/// @brief Состояние энкодера
typedef struct EncoderState {
  EncoderEventType type;
  int8_t steps;             // Вращение
  uint8_t press;            // Нажата ли кнопка
  uint32_t pressDurationMs; // Время нажатия кнопки
} EncoderState;

/// @brief Источники событий для менеджера
typedef enum ManagerEventSource {
  ENCODER = 0,
  PUMP,
  SERVO,
  GLASS,
  WORKER,
} ManagerEventSource;

/// @brief Событие для менеджера
typedef struct ManagerEvent {
  ManagerEventSource source;
  union {
    EncoderState encoder; // от энеодера
    uint8_t pump;         // 1 включено, 0 выключено
    uint8_t servo;        // угол поворота сервы
    // AppStateType_t newState; // новыое состояниу
  } data;
} ManagerEvent;

/// @brief Тип событий для воркера
typedef enum WorkerEventType {
  RUN = 0,
  STOP,
} WorkerEventType;

/// @brief Событие длдя воркера
typedef struct WorkerEvent {
  WorkerEventType type;
  uint8_t angle; // угол на который повернутся
  uint32_t time; // время которое надо лить
} WorkerEvent;

#ifdef __cplusplus
}
#endif

#endif /* _APP_SHARED_H */