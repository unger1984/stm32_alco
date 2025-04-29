#ifndef _MENU_H
#define _MENU_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MenuItem {
  const char *name;
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