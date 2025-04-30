#pragma once

#include "MenuItem.h"

class MenuManager {
public:
  MenuManager(MenuItem *current = &menuMain, uint8_t index = 0,
              bool isSelected = false)
      : current(current), index(index), selected(isSelected) {}

  MenuItem *getCurrent();
  uint8_t getIndex();
  bool isSelected();

  void onClick();
  void onLongPress(uint32_t duration);
  void onHold(uint32_t duration);
  void onRotate(int steps, bool pressed);

private:
  MenuItem *current = nullptr;
  uint8_t index = 0;
  bool selected = false;
  void changeCurrent(MenuItem *item);
};

extern MenuManager appManuManager;