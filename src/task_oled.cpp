#include "task_oled.h"
#include "app_shared.h"
#include <stdio.h>

OLED oled;
char txt[128];

void showMain();
void showCalibration();

void taskOled_Init(void) {
  oled.init();
  oled.clearAll();

  // oled.setFont(u8g2_font_cu12_t_cyrillic);
  // snprintf(txt, sizeof(txt), "НАЛИВАТОР");

  // oled.print(20, 40, txt);
  // oled.update();
}

void taskOled_Run(void) {
  osStatus_t status = osSemaphoreAcquire(appStateMutexHandle, osWaitForever);
  if (status == osOK) {
    if (appState.oledUpdated == 0) {
      oled.clear();
      switch (appState.mode) {
      case MODE_MAIN:
        showMain();
        break;
      case MODE_CALIBRATION:
        showCalibration();
        break;
      default:
        break;
      }
      oled.update();
      appState.oledUpdated = 1;
    }
    osSemaphoreRelease(appStateMutexHandle);
  }
  // osDelay(100);
}

void showMain() {
  oled.setFont(u8g2_font_cu12_t_cyrillic);
  snprintf(txt, sizeof(txt), "НАЛИВАТОР");
  oled.print(20, 40, txt);
}

// Структура для хранения текущего состояния меню
typedef struct {
  int current_item; // Индекс текущего элемента
  int top_item;     // Индекс верхнего элемента на экране
} MenuState_t;

void showCalibration() {
  printf("Mode: %d. MenuIndex: %d. Selected: %d\n", appState.mode,
         appState.menuState.index, appState.menuState.selected);

  oled.setFont(u8g2_font_unifont_t_cyrillic);
  for (int i = 0; i < 4; i++) {
    if (appState.menuState.top_index + i >= appState.menuState.size) {
      break;
    }
    int selected = appState.menuState.index - appState.menuState.top_index;

    oled.setColor(1);
    if (selected == i) {
      if (appState.menuState.selected) {
        oled.box(0, (i * 16), 128, 15);
        oled.setColor(0);
      } else {
        oled.frame(0, (i * 16), 128, 15);
      }
    }

    oled.print(2, 12 + (i * 16),
               menuCalibration[appState.menuState.top_index + i]);
  }
}