#include "AppContext.h"
#include <cmsis_os2.h>
#include <stdio.h>

void TaskManager(void *argument) {

  app.switchState(&appStateIdle);

  // currentState = {
  //     .type = IDLE,
  //     .servo = 0,
  //     .pump = 0,
  //     .hold = 0,
  //     .menu =
  //         {
  //             .current = NULL,
  //             .index = 0,
  //             .isSelected = 0,
  //         },
  //     .currentSettings = &currentSettings,
  // };

  for (;;) {
    ManagerEvent event;
    osStatus_t status = osMessageQueueGet(queueManagerHandle, &event, NULL, 0);
    if (status == osStatus_t::osOK) {
      app.handleEvent(event);
    }
    osDelay(1);
  }
}