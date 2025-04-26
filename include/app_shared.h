#ifndef APP_SHARED_H
#define APP_SHARED_H

#include "cmsis_os.h"
#include <event_groups.h>
#include <semphr.h>

/// Режимы приложения
typedef enum {
  MODE_AUTO = 0,
  MODE_MANUAL,
  MODE_DRAIN,
  MODE_CALIBRATION,
} AppMode;

/// Настройки положения серво
typedef struct {
  int r1, r2, r3, r4, r5, r6;
} AppSettingsServo;

/// Настройки дозировок по рюмкам
typedef struct {
  int r1, r2, r3, r4, r5, r6;
} AppSettingsDosage;

/// Настройки приложения
typedef struct {
  AppSettingsServo servo;
  int pump;
  AppSettingsDosage dosage;
} AppSettings;

/// Тип пункта меню
typedef enum {
  Menu = 0,
  Action,
  Settings,
} MenuItemType;

typedef struct MenuItem MenuItem;

// Пункт меню
struct MenuItem {
  const char *label;
  MenuItemType type;
  const MenuItem *subItems;
  uint8_t size;
  int *value;
  int min, max;
  const MenuItem *parent; // указатель на родителя
};

/// Состояние меню
typedef struct {
  const MenuItem *active; // Меню в котором находимся
  int8_t index;           // Индекс текущего элемента подменю
  uint8_t top_index;      // Индекс верхнего элемента на экране
  uint8_t selected;       // Выбран ли пункт на котором индекс
} MenuState;

// Состояние приложения
typedef struct {
  AppMode mode;
  uint16_t servoAngle;
  uint32_t timer;
  MenuState menuState;
} AppState;

extern volatile AppState appState;
extern osSemaphoreId_t appStateMutexHandle;
extern osEventFlagsId_t updateEventHandle;

extern const MenuItem menuCalibration[3];
extern const MenuItem mainMenu;

#endif /* APP_SHARED_H */