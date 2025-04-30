#pragma once

#include <cstdint>

// Класс углов для серво
class GlassAngles {
public:
  uint8_t a1 = 15, a2 = 45, a3 = 75, a4 = 105, a5 = 135, a6 = 165;

  GlassAngles() = default;
};

// Класс доз для каждой позиции
class GlassDoses {
public:
  uint8_t d1 = 100, d2 = 100, d3 = 100, d4 = 100, d5 = 100, d6 = 100;

  GlassDoses() = default;
};

// Основной класс настроек
class AppSettings {
public:
  uint32_t calibration = 0; // время налива стопки
  GlassAngles angles;
  GlassDoses doses;
  uint8_t isDebug = false;

  AppSettings() = default;
};

extern AppSettings appSettings;
