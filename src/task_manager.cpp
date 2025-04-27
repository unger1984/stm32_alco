#include "app.h"
#include <cmsis_os2.h>
#include <stdio.h>

struct {
  uint8_t encbtn = 0;
  uint8_t pump = 0;
  uint8_t servo = 0;
} AppState;

void TaskManager(void *argument) {

  for (;;) {
    ManagerEvent_t event;
    osStatus_t status = osMessageQueueGet(queueManagerHandle, &event, NULL, 0);
    if (status == osStatus_t::osOK) {
      //   printf("SOURCE %d Event: %d Steps: %d Pressed %d Durations: %lu\r\n",
      //          event.source, event.data.encoder.type,
      //          event.data.encoder.steps, event.data.encoder.press,
      //          event.data.encoder.pressDurationMs);
      switch (event.source) {
      case ManagerEventSource_t::ENCODER: {
        switch (event.data.encoder.type) {
        case EncoderEvent_t::Press: {
          AppState.encbtn = 1;
          uint8_t cmd = 1;
          osMessageQueuePut(queuePumpHandle, &cmd, 0, osWaitForever);
        } break;
        case EncoderEvent_t::Release: {
          AppState.encbtn = 0;
          uint8_t cmd = 0;
          osMessageQueuePut(queuePumpHandle, &cmd, 0, osWaitForever);
        } break;
        case EncoderEvent_t::Rotate: {
          int16_t angle = AppState.servo + event.data.encoder.steps;
          if (angle > 180)
            angle = 180;
          if (angle < 0)
            angle = 0;
          osMessageQueuePut(queueServoHandle, &angle, 0, osWaitForever);
        } break;
        default:
          break;
        }
      } break;
      case ManagerEventSource_t::PUMP:
        AppState.pump = event.data.pump;
        printf("Pump %d\r\n", AppState.pump);
        break;
      case ManagerEventSource_t::SERVO:
        AppState.servo = event.data.angle;
        printf("Servo angle %d\r\n", AppState.servo);
      default:
        break;
      }

      osDelay(1);
    }
  }
}