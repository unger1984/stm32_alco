#pragma once

#include <stdint.h>

#define MENU_DRAIN "Прокачка"
#define MENU_DOSAGE "Дозы по стопкам"
#define MENU_SERVO "Углы серво"
#define MENU_CALIBRATION "Калибровка"
#define MENU_DEBUG "Отладка"

#ifndef _MENU_H
#define _MENU_H

enum class MenuItemType { MENU = 0, ACTION, EDIT };

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

extern MenuItem menuMain;
extern MenuItem menuDrain;
extern MenuItem menuDosage;
extern MenuItem menuServo;
extern MenuItem menuCalibration;
extern MenuItem menuDebug;

#endif /* _MENU_H */