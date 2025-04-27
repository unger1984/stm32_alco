#include "app.h"
#include "gpio.h"
#include <cmsis_os2.h>
#include <stdio.h>

void sendPumpStatus(uint8_t pump) {
  ManagerEvent_t event = {
      .source = PUMP,
      .data =
          {
              .pump = pump,
          },
  };
  osMessageQueuePut(queueManagerHandle, &event, 0, osWaitForever);
}

void TaskPump(void *argument) {
  uint8_t status = 0;
  HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PinState::GPIO_PIN_RESET);
  sendPumpStatus(status);

  for (;;) {
    uint8_t cmd;
    osStatus_t wait =
        osMessageQueueGet(queuePumpHandle, &cmd, NULL, osWaitForever);
    if (wait == osStatus_t::osOK) {
      if (status != cmd) {
        HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin,
                          cmd ? GPIO_PinState::GPIO_PIN_SET
                              : GPIO_PinState::GPIO_PIN_RESET);
        status = cmd;
      }
      sendPumpStatus(cmd);
    }
  }
}