#pragma once

#include <stdint.h>

// Основной класс настроек
class AppSettings {
public:
  uint32_t calibration = 0; // время налива стопки
  uint8_t angles[6] = {15, 45, 75, 105, 135, 165};
  uint8_t doses[6] = {100, 100, 100, 100, 100, 100};
  uint8_t isDebug = false;

  AppSettings() = default;
};

extern AppSettings appSettings;
