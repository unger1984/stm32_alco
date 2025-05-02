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
extern osMessageQueueId_t queueHardwareHandle;
extern osMessageQueueId_t queueWorkerHandle;
extern osThreadId_t taskDisplayHandle;

void TaskEncoder(void *argument);
void TaskManager(void *argument);
void TaskHardware(void *argument);
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
  FROM_ENCODER = 0,
  FROM_PUMP,
  FROM_SERVO,
  FROM_GLASS,
  FROM_WORKER,
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

typedef enum HardwareEventType {
  TO_PUMP = 0,
  TO_SERVO,
  TOLEDS,
} HardwareEventType;

typedef struct HardwareEvent {
  HardwareEventType type;
  union {
    uint8_t pump;  // 1 включено, 0 выключено
    uint8_t servo; // угол поворота сервы
    // AppStateType_t newState; // новыое состояниу
  } data;
} HardwareEvent;

typedef struct AppSettings {
  uint32_t MagicNum;
  uint32_t calibration; // = 0; // время налива стопки
  uint8_t angles[6];    // = {15, 45, 75, 105, 135, 165};
  uint8_t doses[6];     // = {100, 100, 100, 100, 100, 100};
  uint8_t isDebug;      // = false;
} AppSettings;

#ifdef __cplusplus
}
#endif

#endif /* _APP_SHARED_H */