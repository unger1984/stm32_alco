#include "AppContext.h"
#include "utils.h"
#include <oled.h>
#include <stdio.h>

#define WIDTH 128
#define HALF_SCREEN WIDTH / 2
#define HALF_TEXT 5
#define MAX_DISPLAY 4

OLED oled;
char txt[128];

void drowIdle();
void drowMenu(MenuManager *menu);
void drowDrain();
void drowCalibration();

void TaskDisplay(void *argument) {

  oled.init();
  drowIdle();

  for (;;) {
    uint32_t flags = osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
    if (flags & 0x01) {
      switch (app.getState()->getType()) {
      case AppStateType::IDLE:
        drowIdle();
        break;
      case AppStateType::MENU:
        drowMenu(app.getMenu());
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

void drowMenu(MenuManager *menu) {
  oled.clear();
  switch (menu->getCurrent()->getType()) {
  case MenuItemType::Menu: {
    uint8_t size = menu->getCurrent()->getSize();
    uint8_t index = menu->getIndex();
    if (size > 0) {
      // Отрисуем дочерние пункты
      uint8_t topIndex = getTopIndex(menu->getIndex(), size);
      oled.setFont(u8g2_font_unifont_t_cyrillic);
      for (int i = 0; i < 4; i++) {
        if (topIndex + i >= size) {
          break;
        }
        int selected = index - topIndex;
        const MenuItem *itemMenu =
            menu->getCurrent()->getChildrent()[topIndex + i];

        oled.setColor(1);
        if (selected == i) {
          if (menu->isSelected()) {
            oled.box(0, (i * 16), WIDTH, 15);
            oled.setColor(0);
          } else {
            oled.frame(0, (i * 16), WIDTH, 15);
          }
        }

        oled.print(2, 12 + (i * 16), itemMenu->getName());

        if (itemMenu->getType() == MenuItemType::Edit) {
          snprintf(txt, sizeof(txt), "%d", *(uint8_t *)itemMenu->getParam());
          oled.print(WIDTH - 2 - utf8_strlen(txt) * 7, 12 + (i * 16), txt);
        }
      }
      oled.update();
    }
  } break;
  case MenuItemType::Action:
    if (app.getMenu()->getCurrent()->isEqual(MENU_DRAIN)) {
      drowDrain();
    } else if (app.getMenu()->getCurrent()->isEqual(MENU_CALIBRATION)) {
      drowCalibration();
    }
    break;
  default:
    break;
  }
}

void drowDrain() {
  oled.clear();
  const char text[] = "ПРОКАЧКА";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 30, txt);

  float time = (float)app.getHold() / 1000;
  int t_int = (int)time;
  // int t_frac = (int)(time * 10) % 10;
  int t_frac = (int)(time * 100) % 100;
  snprintf(txt, sizeof(txt), "%d.%02dс", t_int, t_frac);
  oled.print(HALF_SCREEN - utf8_strlen(txt) * HALF_TEXT, 55, txt);
  oled.update();
}

void drowCalibration() {
  oled.clear();
  const char text[] = "КАЛИБРОВКА";
  oled.setFont(u8g2_font_10x20_t_cyrillic);
  snprintf(txt, sizeof(txt), text);
  oled.print(HALF_SCREEN - utf8_strlen(text) * HALF_TEXT, 30, txt);

  float time = (float)app.getHold() / 1000;
  int t_int = (int)time;
  int t_frac = (int)(time * 100) % 100;
  snprintf(txt, sizeof(txt), "%d.%02dс", t_int, t_frac);
  oled.print(HALF_SCREEN - utf8_strlen(txt) * HALF_TEXT, 55, txt);
  oled.update();
}