#ifndef _APP_H
#define _APP_H

#include <cmsis_os.h>
#include <stdint.h>

#include "menu.h"
#include "settings.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  None,    // нет событий
  Rotate,  // вращение
  Press,   // Начало нажатия кнопки
  Release, // Отпускание кнопки
  Hold,    // Удержание кнопки
} EncoderEvent_t;

typedef struct {
  EncoderEvent_t type;
  int8_t steps;             // Вращение
  uint8_t press;            // Нажата ли кнопка
  uint32_t pressDurationMs; // Время нажатия кнопки
} EncoderState_t;

typedef enum {
  ENCODER = 0,
  PUMP,
  SERVO,
  GLASS,
  // STATE,
} ManagerEventSource_t;

/// @brief Глобальное Тип состояния
typedef enum AppStateType {
  IDLE = 0,
  WORK,
  MENU,
} AppStateType_t;

typedef struct {
  ManagerEventSource_t source;
  union {
    EncoderState_t encoder; // от энеодера
    uint8_t pump;           // 1 включено, 0 выключено
    uint8_t servo;          // угол поворота сервы
    // AppStateType_t newState; // новыое состояниу
  } data;
} ManagerEvent_t;

typedef struct {
  AppStateType_t type;
  uint8_t servo;
  uint8_t pump;
  uint32_t hold;
  struct MenuState menu;
  Settings_t *currentSettings;
} AppState;

extern AppState currentState;

extern osMessageQueueId_t queueManagerHandle;
extern osMessageQueueId_t queuePumpHandle;
extern osMessageQueueId_t queueServoHandle;
extern osThreadId_t taskDisplayHandle;
void TaskEncoder(void *argument);
void TaskManager(void *argument);
void TaskPump(void *argument);
void TaskServo(void *argument);
void TaskDisplay(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* _APP_H */