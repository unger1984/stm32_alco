#ifndef _MENU_H
#define _MENU_H

#include <stddef.h>
#include <stdint.h>

#define MENU_DRAIN "Прокачка"
#define MENU_CALIBRATION "Калибровка"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum MenuItemType {
  Menu = 0,
  Action,
  Settings,
} MenuItemType_t;

typedef struct MenuItem {
  const char *name;
  MenuItemType_t type;
  struct MenuItem *parent;
  struct MenuItem **children;
  uint8_t size;
} MenuItem_t;

typedef struct MenuState {
  struct MenuItem *current;
  uint8_t index;
  uint8_t isSelected;
} MenuState_t;

extern MenuItem_t menuMain;
extern MenuItem_t menuDrain;
extern MenuItem_t menuDosage;
extern MenuItem_t menuServo;
extern MenuItem_t menuCalibration;

#ifdef __cplusplus
}
#endif

#endif /* _MENU_H */