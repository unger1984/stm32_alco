#include "app.h"
#include <oled.h>
#include <stdio.h>

#define WIDTH 128
#define HALF_SCREEN WIDTH / 2
#define HALF_TEXT 5
#define MAX_DISPLAY 4

OLED oled;
char txt[128];

size_t utf8_strlen(const char *str);
void drowIdle();
void drowMenu(MenuState_t state);

void TaskDisplay(void *argument) {

  oled.init();
  drowIdle();

  for (;;) {
    uint32_t flags = osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
    if (flags & 0x01) {
      switch (currentState.type) {
      case AppStateType_t::Idle:
        drowIdle();
        break;
      case AppStateType_t::Menu:
        drowMenu(currentState.menu);
        break;
      default:
        break;
      }
    }
  }
}

void drowIdle() {
  oled.clearAll();
  const char text[] = "НАЛИВАТОР";
  const char ver[] = "v1.0";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 30, txt);
  snprintf(txt, sizeof(txt), ver);
  oled.print(HALF_SCREEN - utf8_strlen(ver) * HALF_TEXT, 50, txt);
  oled.update();
}

uint8_t getTopIndex(uint8_t index, uint8_t totalItems) {
  if (totalItems <= MAX_DISPLAY) {
    return 0;
  }

  if (index <= MAX_DISPLAY - 1) {
    // первый экран
    return 0;
  } else if (index >= totalItems - 1) {
    // последний экран
    return totalItems - MAX_DISPLAY;
  } else {
    return index - MAX_DISPLAY + 1;
  }
}

void drowMenu(MenuState_t state) {
  oled.clear();
  if (state.current->size > 0) {
    // Отрисуем дочерние пункты
    uint8_t topIndex = getTopIndex(state.index, state.current->size);
    oled.setFont(u8g2_font_unifont_t_cyrillic);
    for (int i = 0; i < 4; i++) {
      if (topIndex + i >= state.current->size) {
        break;
      }
      int selected = state.index - topIndex;
      const MenuItem *itemMenu = state.current->children[topIndex + i];

      oled.setColor(1);
      if (selected == i) {
        oled.frame(0, (i * 16), WIDTH, 15);
      }

      oled.print(2, 12 + (i * 16), itemMenu->name);
    }
  }
  oled.update();
}

size_t utf8_strlen(const char *str) {
  size_t len = 0;
  while (*str) {
    if ((*str & 0xC0) != 0x80)
      len++; // Считаем только стартовые байты символов
    str++;
  }
  return len;
}