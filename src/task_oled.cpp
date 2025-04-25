#include "task_oled.h"
#include "app_shared.h"
#include <stdio.h>

#define WIDTH 128
#define HALF_SCREEN WIDTH / 2
#define HALF_TEXT 5

OLED oled;
char txt[128];

void showAuto();
void showManual();
void showDrain();
void showCalibration();

size_t utf8_strlen(const char *str) {
  size_t len = 0;
  while (*str) {
    if ((*str & 0xC0) != 0x80)
      len++; // Считаем только стартовые байты символов
    str++;
  }
  return len;
}

void taskOled_Init(void) {
  oled.init();
  oled.clearAll();

  const char text[] = "НАЛИВАТОРv0.1";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 40, txt);
  oled.update();
}

void taskOled_Run(void) {
  osStatus_t status = osSemaphoreAcquire(appStateMutexHandle, osWaitForever);
  if (status == osOK) {
    if (appState.oledUpdated == 0) {
      oled.clear();
      switch (appState.mode) {
      case MODE_AUTO:
        showAuto();
        break;
      case MODE_MANUAL:
        showManual();
        break;
      case MODE_DRAIN:
        showDrain();
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

void showAuto() {
  const char text[] = "АВТОМАТ";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 40, txt);
}

void showManual() {
  const char text[] = "РУЧНОЙ";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 40, txt);
}

void showDrain() {
  const char text[] = "ДРЕНАЖ";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 40, txt);

  if (appState.timer > 0) {
    oled.setFont(u8g2_font_unifont_t_cyrillic);
    float time = (float)appState.timer / 1000;
    int t_int = (int)time;
    int t_frac = (int)(time * 10) % 10;
    snprintf(txt, sizeof(txt), "%d.%dс", t_int, t_frac);
    oled.print(HALF_SCREEN - utf8_strlen(text) * 2, 60, txt);
  }
}

void drowMenu(MenuState menu) {}

void showCalibration() {
  // TODO удалить отладку
  printf("Mode: %d. Active: %s, MenuIndex: %d. Selected: %d\n", appState.mode,
         appState.menuState.active->label, appState.menuState.index,
         appState.menuState.selected);

  oled.setFont(u8g2_font_unifont_t_cyrillic);
  for (int i = 0; i < 4; i++) {
    if (appState.menuState.top_index + i >= appState.menuState.active->size) {
      break;
    }
    int selected = appState.menuState.index - appState.menuState.top_index;
    const MenuItem *itemMenu =
        &appState.menuState.active->subItems[appState.menuState.top_index + i];

    oled.setColor(1);
    if (selected == i) {
      if (appState.menuState.selected) {
        oled.box(0, (i * 16), WIDTH, 15);
        oled.setColor(0);
      } else {
        oled.frame(0, (i * 16), WIDTH, 15);
      }
    }

    oled.print(2, 12 + (i * 16), itemMenu->label);

    if (itemMenu->type == MenuItemType::Settings) {
      snprintf(txt, sizeof(txt), "%d", *(itemMenu->value));
      oled.print(WIDTH - 2 - utf8_strlen(txt) * 7, 12 + (i * 16), txt);
    }
  }
}