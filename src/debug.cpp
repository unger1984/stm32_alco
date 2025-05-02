#include "debug.h"
#include <stdio.h>

osTimerId_t debugTimer = osTimerNew(debugPrint, osTimerPeriodic, NULL, NULL);

void debugPrint(void *argument) {
  char stats[512];
  printf("---------------------------------\n");
  vTaskGetRunTimeStats(stats);
  printf("%s\n", stats);

  UBaseType_t taskCount = uxTaskGetNumberOfTasks();
  TaskStatus_t *taskStatusArray;

  taskStatusArray = reinterpret_cast<TaskStatus_t *>(
      pvPortMalloc(taskCount * sizeof(TaskStatus_t)));
  if (taskStatusArray == NULL) {
    printf("malloc failed\n");
    return;
  }

  taskCount = uxTaskGetSystemState(taskStatusArray, taskCount, NULL);
  for (UBaseType_t i = 0; i < taskCount; i++) {
    printf("%s\t\t%lu\n", taskStatusArray[i].pcTaskName,
           uxTaskGetStackHighWaterMark(taskStatusArray[i].xHandle));
  }

  vPortFree(taskStatusArray);
}

void handleDebug(uint8_t isDebug) {
  if (isDebug <= 0) {
    osTimerStop(debugTimer);
  } else {
    osTimerStart(debugTimer, 2000);
  }
}