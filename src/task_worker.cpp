#include "AppContext.h"
#include <stdio.h>

#define IDLE_TIMEOUT 10

void TaskWorker(void *argument) {
  uint32_t timeout = IDLE_TIMEOUT;
  for (;;) {
    WorkerEvent event;
    /// Ждем нового события или до истечения таймаута
    osStatus_t status =
        osMessageQueueGet(queueWorkerHandle, &event, NULL, timeout);
    if (status == osStatus_t::osOK) {
      printf("event\r\n");
      switch (event.type) {
      case WorkerEventType::RUN:
        if (event.time == 0) {
          // нет задач, значит надо запарковаться
          app.updatePump(false);
          app.updateServo(0);
        } else {
          // запустим задачу на заданное время
          app.updateServo(event.angle);
          timeout = event.time;
          app.updatePump(true);
        }
        break;
      case WorkerEventType::STOP:
        // остановим текущее задание
        app.updatePump(false);
        app.sendWorkerDone();
        timeout = IDLE_TIMEOUT;
        break;
      }
    } else if (status == osStatus_t::osErrorTimeout && timeout > IDLE_TIMEOUT) {
      // истек таймаут
      app.updatePump(false);
      app.sendWorkerDone();
      timeout = IDLE_TIMEOUT;
    }
  }
}
