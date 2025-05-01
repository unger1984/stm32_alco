#include "AppContext.h"

void TaskManager(void *argument) {

  app.switchState(&appStateLoading);

  for (;;) {
    ManagerEvent event;
    osStatus_t status = osMessageQueueGet(queueManagerHandle, &event, NULL, 0);
    if (status == osStatus_t::osOK) {
      app.handleEvent(event);
    }
    osDelay(1);
  }
}