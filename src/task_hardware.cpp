#include "AppContext.h"
#include "app_shared.h"
#include "gpio.h"
#include "tim.h"
#include "utils.h"

void setAngle(uint8_t angle) {
  angle = clamp<uint8_t>(angle, 0, 180);

  uint16_t pulse =
      SERVO_MIN + ((uint32_t)(SERVO_MAX - SERVO_MIN) * angle) / 180;
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pulse);
}

void TaskHardware(void *argument) {
  bool pumpStatus = 0;
  HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin, GPIO_PinState::GPIO_PIN_RESET);
  app.sendPumpStatus(pumpStatus);

  uint8_t servoAngle = 0;
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
  setAngle(servoAngle);
  app.sendServoStatus(servoAngle);

  for (;;) {
    HardwareEvent event;
    osStatus_t wait =
        osMessageQueueGet(queueHardwareHandle, &event, NULL, osWaitForever);
    if (wait == osStatus_t::osOK) {
      switch (event.type) {
      case HardwareEventType::TO_PUMP:
        if (pumpStatus != event.data.pump) {
          HAL_GPIO_WritePin(PUMP_GPIO_Port, PUMP_Pin,
                            event.data.pump ? GPIO_PinState::GPIO_PIN_SET
                                            : GPIO_PinState::GPIO_PIN_RESET);
          pumpStatus = event.data.pump;
        }
        app.sendPumpStatus(event.data.pump);
        break;
      case HardwareEventType::TO_SERVO:
        if (servoAngle != event.data.servo) {
          setAngle(event.data.servo);
          osDelay(WAIT_SERVO); // серво поварачивается не мгновенно!
          servoAngle = event.data.servo;
        }
        app.sendServoStatus(event.data.servo);
        break;
      default:
        break;
      }
    }
  }
}