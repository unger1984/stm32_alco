#pragma once

#include <stdint.h>

#define MENU_DRAIN "Прокачка"
#define MENU_DOSAGE "Дозы по стопкам"
#define MENU_SERVO "Углы серво"
#define MENU_CALIBRATION "Калибровка"

#ifndef _MENU_H
#define _MENU_H

enum class MenuItemType { Menu = 0, Action, Edit };

class MenuItem {
public:
  MenuItem(const char *name, MenuItemType type, MenuItem *parent = nullptr,
           MenuItem **children = nullptr, uint8_t size = 0,
           void *settingsPtr = nullptr)
      : name(name), type(type), parent(parent), children(children), size(size),
        settingsPtr(settingsPtr) {}

  const char *getName() const;
  MenuItemType getType() const;
  MenuItem *getParent() const;
  MenuItem **getChildrent() const;
  uint8_t getSize() const;
  bool isEqual(const char *value);
  void *getParam() const;

private:
  const char *name;
  MenuItemType type;
  MenuItem *parent = nullptr;
  MenuItem **children = nullptr;
  uint8_t size = 0;
  void *settingsPtr = nullptr;
};

class MenuState {
public:
  MenuState(MenuItem *current = nullptr, uint8_t index = 0,
            bool isSelected = false)
      : current(current), index(index), selected(isSelected) {}

  void setCurrent(MenuItem *menu);
  MenuItem *getCurrent();
  uint8_t getIndex();
  void setIndex(uint8_t value);
  bool isSelected();
  void setSelected(bool value);

private:
  MenuItem *current = nullptr;
  uint8_t index = 0;
  bool selected = false;
};

extern MenuState menuState;

extern MenuItem menuMain;
extern MenuItem menuDrain;
extern MenuItem menuDosage;
extern MenuItem menuServo;
extern MenuItem menuCalibration;

#endif /* _MENU_H */