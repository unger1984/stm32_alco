#ifndef APP_SHARED_H
#define APP_SHARED_H

#include "cmsis_os.h"
#include <semphr.h>

typedef enum {
  MODE_MAIN = 0,
  MODE_CALIBRATION,
  MODE_CALIBRATION_SERVO,
  MODE_CALIBRATION_SERVO_EDIT
} AppMode;

typedef struct {
  int8_t index;      // Индекс текущего элемента
  uint8_t top_index; // Индекс верхнего элемента на экране
  uint8_t selected;
  uint8_t size;
} MenuState;

typedef struct {
  AppMode mode;
  uint8_t oledUpdated;
  MenuState menuState;
} AppState;

extern volatile AppState appState;
extern osSemaphoreId_t appStateMutexHandle;

extern const char *const menuCalibration[];
extern const uint8_t menuCalibrationSize;

#endif