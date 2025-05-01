#include "AppContext.h"
#include "app_shared.h"
#include "gpio.h"

void TaskPump(void *argument) {
  bool status = 0;
  HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PinState::GPIO_PIN_RESET);
  app.sendPumpStatus(status);

  for (;;) {
    bool cmd;
    osStatus_t wait =
        osMessageQueueGet(queuePumpHandle, &cmd, NULL, osWaitForever);
    if (wait == osStatus_t::osOK) {
      if (status != cmd) {
        HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin,
                          cmd ? GPIO_PinState::GPIO_PIN_SET
                              : GPIO_PinState::GPIO_PIN_RESET);
        status = cmd;
      }
      app.sendPumpStatus(cmd);
    }
  }
}