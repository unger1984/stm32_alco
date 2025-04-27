#ifndef _APP_H
#define _APP_H

#include <cmsis_os.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define millis() ((osKernelGetTickCount() * 1000U) / osKernelGetTickFreq())

typedef enum {
  None,    // нет событий
  Rotate,  // вращение
  Press,   // Начало нажатия кнопки
  Release, // Отпускание кнопки
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
} ManagerEventSource_t;

typedef struct {
  ManagerEventSource_t source;
  union {
    EncoderState_t encoder; // от энеодера
    uint8_t pump;           // 1 включено, 0 выключено
    uint8_t angle;          // угол поворота сервы
  } data;
} ManagerEvent_t;

extern osMessageQueueId_t queueManagerHandle;
extern osMessageQueueId_t queuePumpHandle;
extern osMessageQueueId_t queueServoHandle;
void TaskEncoder(void *argument);
void TaskManager(void *argument);
void TaskPump(void *argument);
void TaskServo(void *argument);

#ifdef __cplusplus
}
#endif

#endif /* _APP_H */