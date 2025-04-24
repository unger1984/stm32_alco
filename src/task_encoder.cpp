#include "task_encoder.h"
#include <stdio.h>

extern TIM_HandleTypeDef htim4;
static Encoder _encoder(&htim4, ENC_BTN_GPIO_Port, ENC_BTN_Pin);

void taskEncoder_Init(void) { _encoder.Init(); }

void taskEncoder_Run(void) {
  _encoder.Update();
  if (_encoder.HasEvent()) {
    EncoderEvent event = _encoder.GetEvent();

    switch (event.type) {
    case EncoderEventType::Rotate:
      printf("Rotate: %d, Button pressed: %d\r\n", event.ticks,
             event.buttonPressedDuring);

      break;
    case EncoderEventType::Click:
      printf("Click, Duration: %lu ms\r\n", event.pressDurationMs);
      break;
    case EncoderEventType::Release:
      printf("Release, Duration: %lu ms\r\n", event.pressDurationMs);
      break;
    default:
      break;
    }
  }
}