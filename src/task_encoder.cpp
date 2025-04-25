#include "task_encoder.h"
#include "app_shared.h"
#include <stdio.h>

#define MENU_VISIBLE_ITEMS 4

extern TIM_HandleTypeDef htim4;
static Encoder _encoder(&htim4, ENC_BTN_GPIO_Port, ENC_BTN_Pin);

void processModeMain(EncoderEvent event);
void processModeCalibration(EncoderEvent event);

void taskEncoder_Init(void) { _encoder.Init(); }

void taskEncoder_Run(void) {
  _encoder.Update();
  if (_encoder.HasEvent()) {
    EncoderEvent event = _encoder.GetEvent();

    osStatus_t status = osSemaphoreAcquire(appStateMutexHandle, osWaitForever);
    if (status == osOK) {
      switch (appState.mode) {
      case MODE_MAIN:
        processModeMain(event);
        break;
      case MODE_CALIBRATION:
        processModeCalibration(event);
        break;
      default:
        break;
      }
      osSemaphoreRelease(appStateMutexHandle);
    }

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
  // osDelay(10);
}

void processModeMain(EncoderEvent event) {
  if (event.type == EncoderEventType::Release && event.pressDurationMs > 500) {
    appState.mode = MODE_CALIBRATION;
    appState.menuState.index = 0;
    appState.menuState.top_index = 0;
    appState.menuState.size = 12;
    appState.oledUpdated = 0;
  }
}

void processModeCalibration(EncoderEvent event) {
  switch (event.type) {
  case EncoderEventType::Rotate:
    appState.menuState.index += event.ticks;

    if (appState.menuState.index < 0) {
      appState.menuState.index = 0;
    }
    if (appState.menuState.index >= appState.menuState.size) {
      appState.menuState.index = appState.menuState.size - 1;
    }

    if (appState.menuState.top_index + MENU_VISIBLE_ITEMS - 1 <
        appState.menuState.index) {
      // если прокрутили ниже чем есть
      appState.menuState.top_index += 1;
    } else if (appState.menuState.index < appState.menuState.top_index) {
      appState.menuState.top_index -= 1;
    }

    appState.oledUpdated = 0;
    break;
  default:
    break;
  }
}