#include "AppContext.h"

#define IDLE_TIMEOUT 10

void TaskWorker(void *argument) {
  uint32_t timeout = IDLE_TIMEOUT;
  for (;;) {
    WorkerEvent event;
    /// Ждем нового события или до истечения таймаута
    osStatus_t status =
        osMessageQueueGet(queueWorkerHandle, &event, NULL, timeout);
    if (status == osStatus_t::osOK) {
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
    } else {
      // истек таймаут
    }
  }
}
