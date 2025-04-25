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
  for (int i = 0; i < 4; i++) {
    int item = appState.menuState.top_index + i;
    int selected = appState.menuState.index - appState.menuState.top_index;
    // if (index >= appState.menuState.size)
    //   break;

    // if (index == appState.menuState.index) {
    // oled.setColor(1); // Выделяем активный элемент
    // } else {
    // oled.setColor(0); // Неактивный элемент
    // }

    snprintf(txt, sizeof(txt), "%s Пункт %d", selected == i ? ">>" : "  ",
             item);
    oled.print(0, 20 + (i * 12), txt);
  }
}