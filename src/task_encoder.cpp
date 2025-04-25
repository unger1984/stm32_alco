#include "task_encoder.h"
#include "app_shared.h"
#include <stdio.h>
#include <string.h>

#define MENU_VISIBLE_ITEMS 4

extern TIM_HandleTypeDef htim4;
static Encoder _encoder(&htim4, ENC_BTN_GPIO_Port, ENC_BTN_Pin);

void processModeAuto(EncoderEvent event);
void processModeManual(EncoderEvent event);
void processModeDrain(EncoderEvent event);
void processModeCalibration(EncoderEvent event);

int compareStrings(const char *a, const char *b) {
  while (*a && *b && *a == *b) {
    a++;
    b++;
  }
  return *a - *b;
}

void taskEncoder_Init(void) { _encoder.Init(); }

void taskEncoder_Run(void) {
  _encoder.Update();
  if (_encoder.HasEvent()) {
    EncoderEvent event = _encoder.GetEvent();

    osStatus_t status = osSemaphoreAcquire(appStateMutexHandle, osWaitForever);
    if (status == osOK) {
      switch (appState.mode) {
      case MODE_AUTO:
        processModeAuto(event);
        break;
      case MODE_MANUAL:
        processModeManual(event);
        break;
      case MODE_DRAIN:
        processModeDrain(event);
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
    case EncoderEventType::Press:
      printf("Press\r\n");
      break;
    case EncoderEventType::Release:
      printf("Push\r\n");
      break;
    case EncoderEventType::Hold:
      printf("Hold, Duration: %lu ms\r\n", event.pressDurationMs);
      break;
    case EncoderEventType::Click:
      printf("Click, Duration: %lu ms\r\n", event.pressDurationMs);
      break;
    case EncoderEventType::LongClick:
      printf("LongClick, Duration: %lu ms\r\n", event.pressDurationMs);
      break;
    default:
      break;
    }
  }
  // osDelay(10);
}

/// Вход в меню
void handleMenu(const MenuItem *menu) {
  appState.mode = MODE_CALIBRATION;
  appState.menuState.active = menu;
  appState.menuState.index = 0;
  appState.menuState.top_index = 0;
  appState.menuState.selected = 0;
  appState.oledUpdated = 0;
  appState.servoUpdated = 0;
  appState.servoAngle = 0;
  appState.timer = 0;
}

/// Переключение режимов главного экрана
void handleRotateMain(int tick) {
  if (tick > 0) {
    switch (appState.mode) {
    case MODE_AUTO:
      appState.mode = MODE_MANUAL;
      break;
    case MODE_MANUAL:
      appState.mode = MODE_DRAIN;
      break;
    case MODE_DRAIN:
      appState.mode = MODE_AUTO;
      break;
    default:
      break;
    }
  } else {
    switch (appState.mode) {
    case MODE_AUTO:
      appState.mode = MODE_DRAIN;
      break;
    case MODE_MANUAL:
      appState.mode = MODE_AUTO;
      break;
    case MODE_DRAIN:
      appState.mode = MODE_MANUAL;
      break;
    default:
      break;
    }
  }
  if (appState.mode == MODE_DRAIN) {
    appState.servoUpdated = 0;
    appState.servoAngle = 90;
  } else {
    appState.servoUpdated = 0;
    appState.servoAngle = 0;
  }
  appState.timer = 0;
  appState.oledUpdated = 0;
}

/// Режим АВТО
void processModeAuto(EncoderEvent event) {
  switch (event.type) {
  case EncoderEventType::LongClick:
    handleMenu(&mainMenu);
    break;
  case EncoderEventType::Rotate:
    handleRotateMain(event.ticks);
    break;
  default:
    break;
  }
}

/// Режим РУЧНОЙ
void processModeManual(EncoderEvent event) {
  switch (event.type) {
  case EncoderEventType::LongClick:
    handleMenu(&mainMenu);
    break;
  case EncoderEventType::Rotate:
    handleRotateMain(event.ticks);
    break;
  default:
    break;
  }
}

/// Режим ДРЕНАЖ
void processModeDrain(EncoderEvent event) {
  switch (event.type) {
  case EncoderEventType::Hold:
    if (event.pressDurationMs > 500) {
      appState.timer = event.pressDurationMs;
      appState.oledUpdated = 0;
      // TODO включить помпу
    }
    break;
  case EncoderEventType::LongClick:
    appState.timer = 0;
    appState.oledUpdated = 0;
    // TODO выключить помпу
    break;
  case EncoderEventType::Rotate:
    handleRotateMain(event.ticks);
    break;
  default:
    break;
  }
}

/// Обработка переключение на следующее/предыдущее меню
void handleRotateMenu(int16_t ticks) {
  appState.menuState.index += ticks;
  if (appState.menuState.index < 0) {
    appState.menuState.index = 0;
  }
  if (appState.menuState.index >= appState.menuState.active->size) {
    appState.menuState.index = appState.menuState.active->size - 1;
  }

  if (appState.menuState.top_index + MENU_VISIBLE_ITEMS - 1 <
      appState.menuState.index) {
    // если прокрутили ниже чем есть
    appState.menuState.top_index += 1;
  } else if (appState.menuState.index < appState.menuState.top_index) {
    appState.menuState.top_index -= 1;
  }
  appState.oledUpdated = 0;
}

/// Режим Калибровка
void processModeCalibration(EncoderEvent event) {
  // пункт над которым находимся
  const MenuItem *indexMenu =
      &appState.menuState.active->subItems[appState.menuState.index];
  switch (indexMenu->type) {
  case MenuItemType::Menu:
    // В это сабменю можно провалиться
    switch (event.type) {
    case EncoderEventType::Rotate:
      handleRotateMenu(event.ticks);
      break;
    case EncoderEventType::Click:
      handleMenu(indexMenu);
      break;
    case EncoderEventType::LongClick:
      if (appState.menuState.active->parent == nullptr) {
        appState.mode = MODE_AUTO;
        appState.oledUpdated = 0;
      } else {
        handleMenu(appState.menuState.active->parent);
      }
      break;
    default:
      break;
    }
    break;
  case MenuItemType::Settings:
    // В этом меню изменяются настроки
    if (appState.menuState.selected) {
      switch (event.type) {
      case EncoderEventType::Rotate:
        *(indexMenu->value) += event.ticks;
        if (*(indexMenu->value) > indexMenu->max) {
          *(indexMenu->value) = indexMenu->max;
        }
        if (*(indexMenu->value) < indexMenu->min) {
          *(indexMenu->value) = indexMenu->min;
        }
        appState.oledUpdated = 0;
        if (compareStrings(appState.menuState.active->label,
                           "Нaстройка серво") == 0) {
          appState.servoUpdated = 0;
          appState.servoAngle = (uint16_t)*(indexMenu->value);
        }
        break;
      case EncoderEventType::Click:
        appState.menuState.selected = !appState.menuState.selected;
        appState.oledUpdated = 0;
        break;
      default:
        break;
      }

    } else {
      switch (event.type) {
      case EncoderEventType::Rotate:
        handleRotateMenu(event.ticks);
        break;
      case EncoderEventType::Click:
        appState.menuState.selected = !appState.menuState.selected;
        appState.oledUpdated = 0;
        break;
      case EncoderEventType::LongClick:
        handleMenu(appState.menuState.active->parent);
        break;
      default:
        break;
      }
    }
    break;
  case MenuItemType::Action:
    // Специальное действие
    switch (event.type) {
    case EncoderEventType::Rotate:
      handleRotateMenu(event.ticks);
      break;
    case EncoderEventType::LongClick:
      handleMenu(appState.menuState.active);
      break;
    default:
      break;
    }
    break;
  }

  // if (appState.menuState.selected) {

  // } else {
  //   switch (event.type) {
  //   case EncoderEventType::Rotate:

  //     break;
  //   // case EncoderEventType::LongClick:
  //   //   appState.mode = MODE_MAIN;
  //   //   appState.oledUpdated = 0;
  //   //   break;
  //   case EncoderEventType::Click:
  //     appState.menuState.selected = appState.menuState.selected ? 0 : 1;
  //     appState.oledUpdated = 0;
  //     break;
  //   default:
  //     break;
  //   }
  // }
}