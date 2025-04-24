#include "task_oled.h"
#include <stdio.h>

OLED oled;
char txt[128];

void taskOled_Init(void) {
  oled.init();

  oled.setFont(u8g2_font_cu12_t_cyrillic);
  snprintf(txt, sizeof(txt), "НАЛИВАТОР");

  oled.print(20, 40, txt);
  oled.update();
}

void taskOled_Run(void) {}